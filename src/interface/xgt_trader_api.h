#ifndef __XGT_TRADER_API_H_
#define __XGT_TRADER_API_H_

#include "interface/xgt_trader_struct.h"

namespace XGT {

class XGTTraderApi {
public:
  static XGTTraderApi* CreateTraderApi(const char* log_dir, const char* server, const int& port);
  virtual void FreeTraderApi() = 0;
  virtual int Login(const XGTLoginRequest&) = 0;
  virtual int Logout(const XGTLogoutRequest&) = 0;
  virtual int SubscribeTopic(const XGTSubscribeRequest&) = 0;
  virtual int InsertOrder(const XGTInsertOrderRequest&) = 0;
  virtual int CancelOrder(const XGTCancelOrderRequest&) = 0;
  virtual int QryAccount(const XGTQueryAccountRequest&) = 0;
  virtual int QryPosition(const XGTQueryPositionRequest&) = 0;
  virtual int QryOrder(const XGTQueryOrderRequest&) = 0;
  virtual int QryTrade(const XGTQueryTradeRequest&) = 0;
};

}// namespace XGT

#endif
