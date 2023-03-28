#include "interface/xgt_trader_api_impl.h"

namespace XGT {

void XGTTraderApiImpl::RegisterSpi(XGTTraderSpi* spi) {
  spi_ = spi;
}

void XGTTraderApiImpl::FreeTraderApi() {
  delete client_;
  delete spi_;
  delete this;
}

int XGTTraderApiImpl::Login(const XGTLoginRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::Logout(const XGTLogouRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::SubscribeTopic(const XGTSubscribeRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::InsertOrder(const XGTInsertOrderRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::CancelOrder(const XGTCancelOrderRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::QryAccount() {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::QryPosition() {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::QryOrder() {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

int XGTTraderApiImpl::QryTrade() {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  return 0;
}

XGTTraderApiImpl::XGTTraderApiImpl(const char* log_dir, const std::string& server, const int& port) {
  client_ = new XGTClient(server, port);
  client_->Connect();
  EpollInstance& epoll_instance = EpollInstance::GetInstance();
  epoll_instance.AddEvent(client_->GetSocket(), client_);
}

XGTTraderApi* XGTTraderApi::CreateTraderApi(const char* log_dir, const std::string& server, const int& port) {
  XGTTraderApi* api = new XGTTraderApiImpl(log_dir, server, port);
  return api;
}

}//namespace XGT
