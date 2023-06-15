#ifndef __EPOLL_STATIC_H_
#define __EPOLL_STATIC_H_

#include <sys/epoll.h>
#include <mutex>
#include <thread>
#include <atomic>

#include "logger/logger.h"
#include "interface/xgt_client.h"

class EpollInstance {
public:
  static EpollInstance& GetInstance() {
    static EpollInstance instance;
    return instance;
  }
  int ep_fd_;
  int AddEvent(int, void*);
  int DeleteEvent(int);
  void EpollLoop();
  EpollInstance(const EpollInstance&) = delete;
  void operator=(const EpollInstance&) = delete;
protected:
  EpollInstance();
  ~EpollInstance();
private:
  static constexpr int event_len_ = 128;
  static constexpr int wait_timeout_ = 10000; // milliseconds
  std::mutex epoll_mutex_;
  std::thread epoll_thread_;
  std::atomic<bool> stop_{false};
};

#endif
