#ifndef __XGT_TRADER_API_IMPL_H_
#define __XGT_TRADER_API_IMPL_H_

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include "interface/xgt_trader_api.h"
#include "interface/epoll_static.h"

namespace XGT {

class XGTTraderApiImpl : public XGTTraderApi {
public:
  explicit XGTTraderApiImpl(const char* log_dir, XGTTraderSpi* trader_spi);
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
private:
  XGTTraderApiImpl() = delete;
  XGTTraderApiImpl(const XGTTraderApiImpl&) = delete;
  int SetNonBlocking(int fd);
  int client_socket_;
};

}//namespace XGT

#endif
