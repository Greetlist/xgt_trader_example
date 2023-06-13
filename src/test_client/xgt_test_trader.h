#ifndef __XGT_TEST_TRADER_H_
#define __XGT_TEST_TRADER_H_

#include "interface/xgt_trader_api.h"
#include "interface/xgt_trader_spi.h"
#include "logger/logger.h"

#include <thread>
#include <unordered_map>
#include <string>
#include <string.h>

class TestTrader : public XGT::XGTTraderSpi {
public:
  TestTrader(std::unordered_map<std::string, std::string>&& config);
  ~TestTrader();
  int Init();
  int SubscribeTopic();
  int InsertOrder();
  int CancelOrder();
  int QueryAccount();
  int QueryPosition();
  int QueryOrder();
  int QueryTrade();
  virtual void OnConnect() override;
  virtual void OnDisConnect() override;
  virtual void OnLogin(XGT::XGTLoginResponse* res) override;
  virtual void OnSubscribeTopic(XGT::XGTSubscribeTopicResponse* res) override;
  virtual void OnRspReturnOrder(XGT::XGTReturnOrder* res) override;
  virtual void OnRspReturnTrade(XGT::XGTReturnTrade* res) override;
  virtual void OnRspReturnAction(XGT::XGTReturnAction* res) override;
  virtual void OnRspQryAccount(XGT::XGTAccountInfo* res) override;
  virtual void OnRspQryPosition(XGT::XGTPositionInfo* position) override;
  virtual void OnRspQryOrder(XGT::XGTOrderInfo* order) override;
  virtual void OnRspQryTrade(XGT::XGTTradeInfo* trade) override;
private:
  XGT::XGTTraderApi* trade_api_;
  std::unordered_map<std::string, std::string> config_;
};

#endif
