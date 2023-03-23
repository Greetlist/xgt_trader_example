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
  client_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  SetNonBlocking(client_fd_);
  EpollInstance& epoll_instance = EpollInstance::GetInstance();
  return epoll_instance.AddEvent(client_fd_);
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

XGTTraderApi* XGTTraderApi::CreateTraderApi(const char* log_dir, XGTTraderSpi* trader_spi) {
  XGTTraderApi* api = new XGTTraderApiImpl(log_dir, trade_spi);
  return api;
}
