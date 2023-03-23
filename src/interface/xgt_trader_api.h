#ifndef __XGT_TRADER_API_H_
#define __XGT_TRADER_API_H_

#include "interface/xgt_trader_struct.h"

namespace XGT {

// CallBack Class
class XGTTraderSpi {
public:
  XGTTraderSpi() = delete;
  XGTTraderSpi(const XGTTraderSpi&) = delete;
  virtual ~XGTTraderSpi() {}

  virtual void OnConnect() {}
  virtual void OnDisConnect() {}
  virtual void OnLogin(XGTLoginResponse* res) {}
  virtual void OnSubscribeTopic(XGTSubscribeTopicResponse* res) {}
  virtual void OnRspReturnOrder(XGTReturnOrder* res) {}
  virtual void OnRspReturnTrade(XGTReturnTrade* res) {}
  virtual void OnRspReturnAction(XGTReturnAction* res) {}
  virtual void OnRspQryAccount(XGTAccountInfo* res) {}
  virtual void OnRspQryPosition(XGTPositionInfo* position) {}
  virtual void OnRspQryOrder(XGTOrderInfo* order) {}
  virtual void OnRspQryTrade(XGTTradeInfo* trade) {}
};

class XGTTraderApi {
public:
  static XGTTraderApi* CreateTraderApi(const char* log_dir, XGTTraderSpi* trader_spi);
  virtual void FreeTraderApi() = 0;
  virtual int Login(const XGTLoginRequest&) = 0;
  virtual int Logout(const XGTLogouRequest&) = 0;
  virtual int SubscribeTopic(const XGTSubscribeRequest&) = 0;
  virtual int InsertOrder(const XGTInsertOrderRequest&) = 0;
  virtual int CancelOrder(const XGTCancelOrderRequest&) = 0;
  virtual int QryAccount() = 0;
  virtual int QryPosition() = 0;
  virtual int QryOrder() = 0;
  virtual int QryTrade() = 0;
};

}// namespace XGT

#endif
