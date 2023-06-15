#include "interface/epoll_static.h"

EpollInstance::EpollInstance() {
  LOG_INFO("Init Epoll");
  ep_fd_ = epoll_create1(EPOLL_CLOEXEC);
  epoll_thread_ = std::thread(&EpollInstance::EpollLoop, this);
}

EpollInstance::~EpollInstance() {
  LOG_INFO("Free Static Instance");
  stop_ = true;
  epoll_thread_.join();
}

int EpollInstance::AddEvent(int socket_fd, void* client) {
  struct epoll_event ev;
  ev.data.ptr = client;
  ev.events = EPOLLIN | EPOLLET;
  std::lock_guard<std::mutex> lk(epoll_mutex_);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
    LOG_ERROR("Epoll Ctl Error");
    return -1;
  }
  return 0;
}

int EpollInstance::DeleteEvent(int socket_fd) {
  std::lock_guard<std::mutex> lk(epoll_mutex_);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_DEL, socket_fd, nullptr) == -1) {
    LOG_ERROR("Epoll Ctl Error");
    return -1;
  }
  return 0;
}

void EpollInstance::EpollLoop() {
  LOG_INFO("Start Epoll Loop");
  while (!stop_) {
    struct epoll_event events[event_len_];
    int num = epoll_wait(ep_fd_, events, event_len_, wait_timeout_);
    for (int i = 0; i < num; ++i) {
      XGT::XGTClient* client = static_cast<XGT::XGTClient*>(events[i].data.ptr);
      client->Read();
    }
  }
}
