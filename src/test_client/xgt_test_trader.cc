#include "xgt_test_trader.h"

TestTrader::TestTrader(std::unordered_map<std::string, std::string>&& config) : config_(config) {
}

TestTrader::~TestTrader() {
  if (trade_api_ != nullptr) {
    trade_api_->FreeTraderApi();
  }
  trade_api_ = nullptr;
}

int TestTrader::Init() {
  trade_api_ = XGT::XGTTraderApi::CreateTraderApi(config_["log_dir"].c_str(), config_["server"].c_str(), std::stoi(config_["port"]));
  if (trade_api_ == nullptr) {
    return -1;
  }
  XGT::XGTLoginRequest req;
  strcpy(req.account, config_["account"].c_str());
  strcpy(req.password, config_["password"].c_str());
  strcpy(req.op_station, config_["op_station"].c_str());
  int res = trade_api_->Login(req);
  return res;
}

int TestTrader::SubscribeTopic() {
  XGT::XGTSubscribeRequest req;
  std::vector<XGT::SubscribeChannel> channel_list{XGT::Order, XGT::Trade};
  for (const auto& channel : channel_list) {
    req.channel = channel;
    int res = trade_api_->SubscribeTopic(req);
    if (res != 0) {
      LOG_ERROR("subscribe topic error");
      return -1;
    }
  }
  return 0;
}

int TestTrader::InsertOrder() {
  //LOG_INFO("%s", __FUNCTION__);
  return 0;
}

int TestTrader::CancelOrder() {
  //LOG_INFO("%s", __FUNCTION__);
  return 0;
}

int TestTrader::QueryAccount() {
  XGT::XGTQueryCommonRequest req;
  strcpy(req.account, config_["account"].c_str());
  strcpy(req.fund_account, config_["fund_account"].c_str());
  int res = trade_api_->QryAccount(req);
  if (res != 0) {
    LOG_ERROR("query account error");
  }
  return 0;
}

int TestTrader::QueryPosition() {
  XGT::XGTQueryCommonRequest req;
  strcpy(req.account, config_["account"].c_str());
  strcpy(req.fund_account, config_["fund_account"].c_str());
  int res = trade_api_->QryPosition(req);
  if (res != 0) {
    LOG_ERROR("query position error");
  }
  return 0;
}

int TestTrader::QueryOrder() {
  XGT::XGTQueryCommonRequest req;
  strcpy(req.account, config_["account"].c_str());
  strcpy(req.fund_account, config_["fund_account"].c_str());
  int res = trade_api_->QryOrder(req);
  if (res != 0) {
    LOG_ERROR("query order error");
  }
  return 0;
}

int TestTrader::QueryTrade() {
  XGT::XGTQueryCommonRequest req;
  strcpy(req.account, config_["account"].c_str());
  strcpy(req.fund_account, config_["fund_account"].c_str());
  int res = trade_api_->QryTrade(req);
  if (res != 0) {
    LOG_ERROR("query trade error");
  }
  return 0;
}

void TestTrader::OnLogin(XGT::XGTLoginResponse* res) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnSubscribeTopic(XGT::XGTSubscribeTopicResponse* res) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspReturnOrder(XGT::XGTReturnOrder* res) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspReturnTrade(XGT::XGTReturnTrade* res) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspReturnAction(XGT::XGTReturnAction* res) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspQryAccount(XGT::XGTAccountInfo* res) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspQryPosition(XGT::XGTPositionInfo* position) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspQryOrder(XGT::XGTOrderInfo* order) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnRspQryTrade(XGT::XGTTradeInfo* trade) {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnConnect() {
  LOG_INFO("%s", __FUNCTION__);
}

void TestTrader::OnDisConnect() {
  LOG_INFO("%s", __FUNCTION__);
}
