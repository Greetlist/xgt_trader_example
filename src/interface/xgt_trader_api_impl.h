#ifndef __XGT_TRADER_API_IMPL_H_
#define __XGT_TRADER_API_IMPL_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>

#include "interface/xgt_trader_api.h"
#include "interface/epoll_static.h"
#include "interface/logger.h"
#include "interface/xgt_client.h"

namespace XGT {

class XGTTraderApiImpl : public XGTTraderApi {
public:
  explicit XGTTraderApiImpl(const char* log_dir, const std::string& server, const int& port);
  void RegisterSpi(XGTTraderSpi* spi);
  virtual void FreeTraderApi();
  virtual int Login(const XGTLoginRequest&) override;
  virtual int Logout(const XGTLogouRequest&) override;
  virtual int SubscribeTopic(const XGTSubscribeRequest&) override;
  virtual int InsertOrder(const XGTInsertOrderRequest&) override;
  virtual int CancelOrder(const XGTCancelOrderRequest&) override;
  virtual int QryAccount() override;
  virtual int QryPosition() override;
  virtual int QryOrder() override;
  virtual int QryTrade() override;
  XGTTraderApiImpl() = delete;
  XGTTraderApiImpl(const XGTTraderApiImpl&) = delete;
private:
  XGTClient* client_;
  XGTTraderSpi* spi_;
};

}//namespace XGT

#endif
