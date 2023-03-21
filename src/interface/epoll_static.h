#ifndef __EPOLL_STATIC_H_
#define __EPOLL_STATIC_H_

#include <sys/epoll.h>

class EpollInstance {
public:
  EpollInstance();
  static EpollInstance* GetInstance();
  int ep_fd_;
private:
  EpollInstance(const EpollInstance&) = delete;
};

static EpollInstance* instance_ = new EpollInstance();

#endif
