#ifndef __XGT_TRADER_SPI_H_
#define __XGT_TRADER_SPI_H_

#include "interface/xgt_trader_struct.h"

namespace XGT {

// CallBack Class
class XGTTraderSpi {
public:
  XGTTraderSpi() {}
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
}// namespace XGT

#endif
