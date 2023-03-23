#include "interface/xgt_trader_api_impl.h"

namespace XGT {

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

int XGTTraderApiImpl::QryAccount() {
}

int XGTTraderApiImpl::QryPosition() {
}

int XGTTraderApiImpl::QryOrder() {
}

int XGTTraderApiImpl::QryTrade() {
}

XGTTraderApiImpl::XGTTraderApiImpl(const char* log_dir, XGTTraderSpi* trader_spi) {
  client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  SetNonBlocking(client_socket_);
  EpollInstance& epoll_instance = EpollInstance::GetInstance();
  epoll_instance.AddEvent(client_socket_, trader_spi);
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
  XGTTraderApi* api = new XGTTraderApiImpl(log_dir, trader_spi);
  return api;
}

}//namespace XGT
