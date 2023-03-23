#include "interface/epoll_static.h"

EpollInstance::EpollInstance() {
  LOG(INFO) << "Init Epoll";
  ep_fd_ = epoll_create1(EPOLL_CLOEXEC);
}

EpollInstance::~EpollInstance() {
  LOG(INFO) << "Free Static Instance";
}

int EpollInstance::AddEvent(int socket_fd) {
  struct epoll_event ev;
  ev.data.ptr = (void*)sp;
  ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
  std::lock_guard<std::mutex> lk(epoll_mutex_);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == -1) {
    LOG(ERROR) << "Epoll Ctl Error";
    return -1;
  }
  return 0;
}

int EpollInstance::DeleteEvent(int socket_fd) {
  std::lock_guard<std::mutex> lk(epoll_mutex_);
  if (epoll_ctl(ep_fd_, EPOLL_CTL_DEL, socket_fd, nullptr) == -1) {
    LOG(ERROR) << "Epoll Ctl Error";
    return -1;
  }
  return 0;
}
