#include "interface/epoll_static.h"

EpollInstance* EpollInstance::GetInstance() {
  return instance_;
}

EpollInstance::EpollInstance() {
  ep_fd_ = epoll_create1(EPOLL_CLOEXEC);
}
