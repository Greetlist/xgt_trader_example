#include "interface/xgt_trader_api_impl.h"

void XGTTraderApiImpl::FreeTraderApi() {
}

int XGTTraderApiImpl::Login(const XGTLoginRequest& req) {
}

int XGTTraderApiImpl::Logout(const XGTLogouRequest& req) {
}
int XGTTraderApiImpl::SubscribeTopic(const XGTSubscribeRequest& req) {
}
int XGTTraderApiImpl::InsertOrder(const XGTInsertOrderRequest& req) {
}
int XGTTraderApiImpl::CancelOrder(const XGTCancelOrderRequest& req) {
}
int XGTTraderApiImpl::QryAccount(const XGTQryAccountRequest& req) {
}
int XGTTraderApiImpl::QryPosition(const XGTQryPositionRequest& req) {
}
int XGTTraderApiImpl::QryOrder(const XGTQryOrderRequest& req) {
}
int XGTTraderApiImpl::QryTrade(const XGTQryTradeRequest& req) {
}

XGTTraderApiImpl::XGTTraderApiImpl(const char* log_dir, XGTTraderSpi* trader_spi) {
  CreateEpoll();
  client_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  SetNonBlocking(client_fd_);
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
  ev.data.ptr = trader_spi;
  if (epoll_ctl(instance->ep_fd_, EPOLL_CTL_ADD, client_fd_, &ev) == -1) {
    perror("epoll_ctl: src_socket");
    return -1;
  }
  return 0;
}

int XGTTraderApiImpl::SetNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int status = fcntl(fd, F_SETFL, flags);
  if (status < 0) {
    perror("File Control Error");
    exit(1);
  }
  return status;
}

void XGTTraderApiImpl::CreateEpoll() {
  ep_fd_ = epoll_create1(EPOLL_CLOEXEC);
  if (n < 0) {
    LOG(ERROR) << "create epoll error";
    exit(-1);
  }
}
