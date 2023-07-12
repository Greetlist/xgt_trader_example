#include "epoll_server/epoll_tcp_server.h"

EpollTCPServer::EpollTCPServer(const EpollRunMode& mode, const EpollTriggerMode& trigger, const int& parallel, const int& msg_processor_num, const std::string& listen_addr, const int& listen_port) : mode_(mode), trigger_mode_(trigger), parallel_num_(parallel), msg_processor_num_(msg_processor_num), listen_addr_(listen_addr), listen_port_(listen_port) {
  stop_ = false;
}

ReturnCode EpollTCPServer::Init() {
  ReturnCode ret = ReturnCode::SUCCESS;
  main_ep_fd_ = epoll_create1(0);
  ret = InitListenSocket();
  message_queue_ = new MPMCQueue<MessageInfo>(1000000);
  return ret;
}

void EpollTCPServer::Start() {
  if (mode_ == EpollRunMode::UseProcess) {
    CreateProcesses();
  } else if (mode_ == EpollRunMode::UseThread) {
    CreateThreads();
  }
  StartMainEpoll();
}

void EpollTCPServer::Stop() {
  stop_ = true;
  if (mode_ == EpollRunMode::UseProcess) {
    for (auto& info : epoll_process_info_vec_) {
      kill(info.process_id, SIGTERM);
      int state;
      waitpid(info.process_id, &state, 0);
    }
  } else if (mode_ == EpollRunMode::UseThread) {
    LOG_INFO("Wait epoll thread finish");
    for (auto& info : epoll_thread_info_vec_) {
      info.thread_instance.join();
    }
    LOG_INFO("Wait message processor finish");
    for (auto& v : msg_processor_vec_) {
      v.join();
    }
    produce_process_msg_.join();
  }
  LOG_INFO("All Thread Quit Successful");
}

ReturnCode EpollTCPServer::InitListenSocket() {
  listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd_ < 0) {
    LOG_ERROR("Socket Error");
    return ReturnCode::CREATE_SOCK_ERROR;
  }

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(listen_port_);
  server.sin_addr.s_addr = inet_addr(listen_addr_.c_str());

  bool flag = true;
  int l = sizeof(flag);
  setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &flag, l);
  int ss = bind(listen_fd_, (struct sockaddr*)&server, sizeof(server));
  if (ss < 0) {
    LOG_ERROR("Bind Error");
    return ReturnCode::BIND_ERROR;
  }
  ss = set_nonblock(listen_fd_);
  if (ss < 0) {
    return ReturnCode::FCNTL_ERROR;
  }

  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.fd = listen_fd_;
  ev.events = EPOLLIN | EPOLLET;
  ss = epoll_ctl(main_ep_fd_, EPOLL_CTL_ADD, listen_fd_, &ev);
  if (ss < 0) {
    LOG_ERROR("Epoll Add Error, error is: %s", strerror(errno));
    return ReturnCode::EP_CONTROL_ERROR;
  }
  LOG_SUCCESS("Create listen socket success");
  return ReturnCode::SUCCESS;
}

void EpollTCPServer::CreateThreads() {
  for (int i = 0; i < parallel_num_; ++i) {
    int cur_pair[2];
    int state = socketpair(AF_UNIX, SOCK_STREAM, 0, cur_pair);
    if (state != 0) {
      LOG_ERROR("socketpair error");
      continue;
    }
    ThreadInfo info;
    info.thread_instance = std::thread(&EpollTCPServer::MainWorker, this, cur_pair[1]);
    info.pair_fd = cur_pair[0];
    epoll_thread_info_vec_.emplace_back(std::move(info));
  }

  for (int i = 0; i < msg_processor_num_; ++i) {
    std::thread msg_process_thread = std::thread(&EpollTCPServer::MainMessageProcessor, this);
    msg_processor_vec_.emplace_back(std::move(msg_process_thread));
  }

  produce_process_msg_ = std::thread(&EpollTCPServer::ProduceProcessInfo, this);
}

void EpollTCPServer::CreateProcesses() {
  pid_t pid;
  for (int i = 0; i < parallel_num_; ++i) {
    int cur_pair[2];
    int state = socketpair(AF_UNIX, SOCK_STREAM, 0, cur_pair);
    if (state != 0) {
      LOG_ERROR("socketpair error");
      continue;
    }
    if ((pid = fork()) == 0) {
      close(cur_pair[0]);
      MainWorker(cur_pair[1]);
      break;
    } else if (pid > 0) { //for parent process
      close(cur_pair[1]);
      ProcessInfo info{pid, cur_pair[0]};
      epoll_process_info_vec_.emplace_back(std::move(info));
    }
  }
}

void EpollTCPServer::MainWorker(int pair_fd) {
  int thread_ep = epoll_create1(0);
  if (thread_ep < 0) {
    LOG_ERROR("Epoll Create Error");
    return;
  }

  std::vector<int> basic_fd{pair_fd};

  //create flush_timer fd to flush socket writer buffer
  TimerFd flush_fd;
  flush_fd.SetTimeout(0, 100 * 1000 * 1000);
  if (flush_fd.GetTimerFd() > 0) {
    basic_fd.push_back(flush_fd.GetTimerFd());
  } else {
    LOG_ERROR("Create flush_fd Error");
  }

  //create check_connection timer to check whether connection is still alive
  TimerFd check_alive_fd;
  check_alive_fd.SetTimeout(30, 0);
  if (check_alive_fd.GetTimerFd() > 0) {
    basic_fd.push_back(check_alive_fd.GetTimerFd());
  } else {
    LOG_ERROR("Create check_alive_fd Error");
  }

  for (auto fd : basic_fd) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ss = epoll_ctl(thread_ep, EPOLL_CTL_ADD, fd, &ev);
    if (ss < 0) {
      LOG_ERROR("Epoll Add Error");
      return;
    }
  }

  //for all tcp_connection in this thread
  std::unordered_map<TcpConnection*, int> conn_map;

  struct epoll_event events[kEventLen];
  while (!stop_) {
    std::vector<TcpConnection*> need_close;
    int num = epoll_wait(thread_ep, events, kEventLen, 1000);
    for (int i = 0; i < num; ++i) {
      if (events[i].data.fd == pair_fd) {
        int client_fd;
        if (mode_ == EpollRunMode::UseProcess) {
          client_fd = recv_fd(pair_fd);
          LOG_DEBUG("Child Process recv fd: %d", client_fd);
        } else if (mode_ == EpollRunMode::UseThread) {
          if (read(pair_fd, &client_fd, sizeof(client_fd)) < 0) {
            LOG_ERROR("Read From pair_fd error: %s", strerror(errno));
            continue;
          }
        }
        LOG_INFO("Recv New Client: [%d]", client_fd);
        if (AcceptClient(thread_ep, client_fd, conn_map) < 0) {
          close(client_fd);
        }
      } else if (events[i].data.fd == check_alive_fd.GetTimerFd()) {
        uint64_t expire;
        int s = read(events[i].data.fd, &expire, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
          LOG_ERROR("Timer fd is Invalid!");
          epoll_ctl(thread_ep, EPOLL_CTL_DEL, check_alive_fd.GetTimerFd(), NULL);
        }
        for (auto& [conn, _] : conn_map) {
          if (conn->IsExpire()) {
            LOG_WARN("Connection [%d] not receive heartbeat over 5Min, Close Connection", conn->GetSocketFd());
            need_close.push_back(conn);
          }
        }
      } else if (events[i].data.fd == flush_fd.GetTimerFd()) {
        uint64_t expire;
        int s = read(events[i].data.fd, &expire, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
          LOG_ERROR("Timer fd is Invalid!");
          epoll_ctl(thread_ep, EPOLL_CTL_DEL, flush_fd.GetTimerFd(), NULL);
        }

        //loop all tcp_connections to check whether its write buffer still have data not transfer
        for (auto& [conn, _] : conn_map) {
          conn->TryFlushWriteBuffer();
        }
      } else {
        TcpConnection* conn = static_cast<TcpConnection*>(events[i].data.ptr);
        if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
          LOG_ERROR("Read From Client Error");
          need_close.push_back(conn);
        } else if (events[i].events & EPOLLIN) {
          do {
            int n_read = conn->Read();
            if (n_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
              LOG_ERROR("Read From Client Error");
              need_close.push_back(conn);
              break;
            } else if (n_read < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
              break;
            } else if (n_read == 0) {
              LOG_INFO("Client: [%d] close connection.", conn->GetSocketFd());
              need_close.push_back(conn);
              break;
            } else if (n_read > 0) {
              std::vector<MessageInfo*> res = conn->ExtractMessage();
              for (auto msg : res) {
                while (!message_queue_->Push(msg)) {}
              }
            }
          } while (trigger_mode_ == EpollTriggerMode::ET);
        } else {
          LOG_WARN("Unhandle epoll event: %d", events[i].events);
        }
      }
    }

    for (TcpConnection* conn : need_close) {
      int socket_fd = conn->GetSocketFd();
      close(socket_fd);
      epoll_ctl(thread_ep, EPOLL_CTL_DEL, socket_fd, NULL);
      delete conn;
      conn_map.erase(conn);
    }
  }
  LOG_INFO("Quit Main Epoll Loop");
}

int EpollTCPServer::AcceptClient(int thread_ep, int client_fd, std::unordered_map<TcpConnection*, int>& conn_map) {
  TcpConnection* new_connection = new TcpConnection(client_fd);
  new_connection->Init();
  struct epoll_event new_ev;
  memset(&new_ev, 0, sizeof(new_ev));
  new_ev.data.ptr = new_connection;

  new_ev.events = EPOLLIN;
  if (trigger_mode_ == EpollTriggerMode::ET) {
    new_ev.events |= EPOLLET;
  }
  if (epoll_ctl(thread_ep, EPOLL_CTL_ADD, client_fd, &new_ev) < 0) {
    LOG_ERROR("Accept Error, error is : %s", strerror(errno));
    delete new_connection;
    return -1;
  }

  std::time_t ts = std::time(nullptr);
  conn_map[new_connection] = ts;
  return 0;
}

void EpollTCPServer::MainMessageProcessor() {
  while (!stop_) {
    MessageInfo* msg_info = message_queue_->Pop();
    if (msg_info) {
      //LOG_INFO("message_type is: %d, message_len: %d, message: %s", msg_info->message_type, msg_info->message_json.size(), msg_info->message_json.c_str());
      try {
        nlohmann::json j = nlohmann::json::parse(msg_info->message_json);
        XGT::XGTRequest req = MessageCoder::JsonToRequest(msg_info->message_type, j);
        std::unique_ptr<BaseHandler> handler = HandlerFactory::GetHandler(msg_info->message_type, req);
        handler->HandleRequest();
        if (msg_info->message_type == XGT::RequestType::HeartbeatRequest) {
          msg_info->conn->SetLastActiveTime(std::time(nullptr)); //can be coredump if connection is deleted before msg Pop out from queue.
        }
      } catch (...) {
        LOG_ERROR("Invalid Message: %s", msg_info->message_json.c_str());
      }

      //remember to free memory
      delete msg_info;
      process_msg_num_++;
    }
    if (message_queue_->Size() == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}

void EpollTCPServer::ProduceProcessInfo() {
  while (!stop_) {
    LOG_INFO("msg queue len is: %ld, total process msg num: %ld", message_queue_->Size(), process_msg_num_.load());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void EpollTCPServer::StartMainEpoll() {
  int state = listen(listen_fd_, kListenBackLog);
  if (state != 0) {
    LOG_ERROR("Listen Error");
    perror("Listen Error");
    Stop();
    return;
  }

  struct epoll_event events[kEventLen];
  LOG_INFO("Start Epoll Loop");
  while (!stop_) {
    int nums = epoll_wait(main_ep_fd_, events, kEventLen, -1);
    for (int i = 0; i < nums; ++i) {
      if (events[i].data.fd == listen_fd_) {
        struct sockaddr_in cli;
        socklen_t len = sizeof(struct sockaddr_in);
        memset(&cli, 0, sizeof(cli));
        int client_fd = 0;
        while ((client_fd = accept(listen_fd_, (struct sockaddr*)&cli, &len)) > 0) {
          LOG_INFO("Accept client: %d", client_fd);
          set_nonblock(client_fd);
          DealClientConnection(client_fd);
        }
        if (client_fd < 0 && errno != EAGAIN) {
          LOG_ERROR("Accept Error");
        }
      } else { // only enter with UseSingleThread Mode
        int client_fd = events[i].data.fd;
        // read data from client_fd;
      }
    }
  }
  LOG_INFO("Quit Main Epoll Loop");
}

int EpollTCPServer::GetNextWorkerIndex() {
  if (current_worker_idx_ == parallel_num_) {
    current_worker_idx_ = 0;
  }
  return current_worker_idx_++;
}

void EpollTCPServer::DealClientConnection(int client_fd) {
  if (mode_ == EpollRunMode::UseProcess) {
    SendToChildProcess(client_fd);
    close(client_fd);
  } else if (mode_ == EpollRunMode::UseThread) {
    SendToChildThread(client_fd);
  } else if (mode_ == EpollRunMode::UseSingleThread) {
    AddToMainEpoll(client_fd);
  }
}

void EpollTCPServer::SendToChildProcess(int client_fd) {
  ProcessInfo& info = epoll_process_info_vec_[GetNextWorkerIndex()];
  send_fd(info.pair_fd, client_fd);
}

void EpollTCPServer::SendToChildThread(int client_fd) {
  ThreadInfo& info = epoll_thread_info_vec_[GetNextWorkerIndex()];
  int status = write(info.pair_fd, &client_fd, sizeof(client_fd));
  if (status <= 0) {
    LOG_ERROR("Write Error");
  }
}

void EpollTCPServer::AddToMainEpoll(int client_fd) {
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.fd = client_fd;
  ev.events = EPOLLIN | EPOLLET;
  int ss = epoll_ctl(main_ep_fd_, EPOLL_CTL_ADD, client_fd, &ev);
  if (ss < 0) {
    LOG_ERROR("Epoll Add Error");
  }
}
