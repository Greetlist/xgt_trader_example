#ifndef __XGT_TRADER_API_IMPL_H_
#define __XGT_TRADER_API_IMPL_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>

#include "interface/xgt_trader_api.h"
#include "interface/xgt_trader_spi.h"
#include "interface/epoll_static.h"
#include "logger/logger.h"
#include "interface/xgt_client.h"

namespace XGT {

class XGTTraderApiImpl : public XGTTraderApi {
public:
  explicit XGTTraderApiImpl(const char* log_dir, const char* server, const int& port);
  void RegisterSpi(XGTTraderSpi* spi);
  virtual void FreeTraderApi();
  virtual int Login(const XGTLoginRequest&) override;
  virtual int Logout(const XGTLogoutRequest&) override;
  virtual int SubscribeTopic(const XGTSubscribeRequest&) override;
  virtual int InsertOrder(const XGTInsertOrderRequest&) override;
  virtual int CancelOrder(const XGTCancelOrderRequest&) override;
  virtual int QryAccount(const XGTQueryAccountRequest&) override;
  virtual int QryPosition(const XGTQueryPositionRequest&) override;
  virtual int QryOrder(const XGTQueryOrderRequest&) override;
  virtual int QryTrade(const XGTQueryTradeRequest&) override;
  XGTTraderApiImpl() = delete;
  XGTTraderApiImpl(const XGTTraderApiImpl&) = delete;
private:
  XGTClient* client_;
};

}//namespace XGT

#endif
