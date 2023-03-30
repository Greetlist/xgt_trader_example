#include "interface/xgt_trader_api_impl.h"

namespace XGT {

void XGTTraderApiImpl::RegisterSpi(XGTTraderSpi* spi) {
  client_->SetSpi(spi);
}

void XGTTraderApiImpl::FreeTraderApi() {
  delete client_;
  delete this;
}

int XGTTraderApiImpl::Login(const XGTLoginRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  client_->Write<XGTLoginRequest>(req);
  return 0;
}

int XGTTraderApiImpl::Logout(const XGTLogoutRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  client_->Write<XGTLogoutRequest>(req);
  return 0;
}

int XGTTraderApiImpl::SubscribeTopic(const XGTSubscribeRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  client_->Write<XGTSubscribeRequest>(req);
  return 0;
}

int XGTTraderApiImpl::InsertOrder(const XGTInsertOrderRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  client_->Write<XGTInsertOrderRequest>(req);
  return 0;
}

int XGTTraderApiImpl::CancelOrder(const XGTCancelOrderRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  client_->Write<XGTCancelOrderRequest>(req);
  return 0;
}

int XGTTraderApiImpl::QryAccount(XGTQueryCommonRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  req.request_type = QueryAccountRequest;
  client_->Write<XGTQueryCommonRequest>(req);
  return 0;
}

int XGTTraderApiImpl::QryPosition(XGTQueryCommonRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  req.request_type = QueryPositionRequest;
  client_->Write<XGTQueryCommonRequest>(req);
  return 0;
}

int XGTTraderApiImpl::QryOrder(XGTQueryCommonRequest& req) {
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  req.request_type = QueryOrderRequest;
  client_->Write<XGTQueryCommonRequest>(req);
  return 0;
}

int XGTTraderApiImpl::QryTrade(XGTQueryCommonRequest& req) {
  req.request_type = QueryTradeRequest;
  LOG_INFO("In Api Impl: %s", __FUNCTION__);
  client_->Write<XGTQueryCommonRequest>(req);
  return 0;
}

XGTTraderApiImpl::XGTTraderApiImpl(const char* log_dir, const char* server, const int& port) {
  client_ = new XGTClient(server, port);
  client_->Connect();
  EpollInstance& epoll_instance = EpollInstance::GetInstance();
  epoll_instance.AddEvent(client_->GetSocket(), client_);
}

XGTTraderApi* XGTTraderApi::CreateTraderApi(const char* log_dir, const char* server, const int& port) {
  XGTTraderApi* api = new XGTTraderApiImpl(log_dir, server, port);
  return api;
}

}//namespace XGT
