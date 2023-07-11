#ifndef __EPOLL_TCP_SERVER_H_
#define __EPOLL_TCP_SERVER_H_

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctime>

#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <tuple>
#include <memory>
#include <unordered_map>

#include "util/fd_util.h"
#include "util/ipc_unix.h"
#include "util/timer_fd.h"
#include "common/return_code.h"
#include "logger/logger.h"
#include "epoll_server/global_def.h"
#include "epoll_server/epoll_server_base.h"
#include "tcp/tcp_connection.h"
#include "queue/mpmc_queue.h"
#include "queue/queue_msg.h"
#include "handler/handler_factory.h"

class EpollTCPServer : public EpollServerBase {
 public:
  explicit EpollTCPServer(const EpollRunMode&, const EpollTriggerMode&, const int&, const int&, const std::string&, const int&);
  ~EpollTCPServer() {}
  virtual ReturnCode Init() override;
  virtual void Start() override;
  virtual void Stop() override;
 private:
  ReturnCode InitListenSocket();
  void CreateThreads();
  void CreateProcesses();
  void MainWorker(int);
  int AcceptClient(int, int, std::unordered_map<TcpConnection*, int>&);
  void MainMessageProcessor();
  void ProduceProcessInfo();
  void StartMainEpoll();
  int GetNextWorkerIndex();
  void DealClientConnection(int);
  void SendToChildProcess(int);
  void SendToChildThread(int);
  void AddToMainEpoll(int);

  std::vector<ThreadInfo> epoll_thread_info_vec_;
  std::vector<ProcessInfo> epoll_process_info_vec_;

  std::vector<std::thread> msg_processor_vec_; //only support for thread mod
  std::thread produce_process_msg_; //only support for thread mod

  std::string listen_addr_;
  int listen_port_;
  EpollRunMode mode_;
  EpollTriggerMode trigger_mode_;
  int listen_fd_;
  int main_ep_fd_;
  int parallel_num_;
  int msg_processor_num_;
  int current_worker_idx_ = 0;
  std::atomic<bool> stop_;
  std::atomic<uint64_t> process_msg_num_{0};
  MPMCQueue<MessageInfo>* message_queue_;
  static constexpr int kEventLen = 128;
  static constexpr int kListenBackLog = 128;
  friend class TcpConnection;
};

#endif
