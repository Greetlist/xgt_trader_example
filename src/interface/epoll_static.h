#ifndef __EPOLL_STATIC_H_
#define __EPOLL_STATIC_H_

#include <sys/epoll.h>
#include <glog/logging.h>
#include <mutex>

class EpollInstance {
public:
  static EpollInstance& GetInstance() {
    static EpollInstance instance;
    return instance;
  }
  int ep_fd_;
  int AddEvent(int, void*);
  int DeleteEvent(int);
  EpollInstance(const EpollInstance&) = delete;
  void operator=(const EpollInstance&) = delete;
protected:
  EpollInstance();
  ~EpollInstance();
private:
  std::mutex epoll_mutex_;
};

#endif
