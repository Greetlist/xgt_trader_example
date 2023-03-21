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

class XGTTraderApiImpl : public XGTTraderApi {
public:
  explicit XGTTraderApiImpl(const char* log_dir, XGTTraderSpi* trader_spi);
  virtual void FreeTraderApi();
  virtual int Login(const XGTLoginRequest&);
  virtual int Logout(const XGTLogouRequest&);
  virtual int SubscribeTopic(const XGTSubscribeRequest&);
  virtual int InsertOrder(const XGTInsertOrderRequest&);
  virtual int CancelOrder(const XGTCancelOrderRequest&);
  virtual int QryAccount(const XGTQryAccountRequest&);
  virtual int QryPosition(const XGTQryPositionRequest&);
  virtual int QryOrder(const XGTQryOrderRequest&);
  virtual int QryTrade(const XGTQryTradeRequest&);
private:
  XGTTraderApiImpl() = delete;
  XGTTraderApiImpl(const XGTTraderApiImpl&) = delete;
  int SetNonBlocking(int fd);
  int CreateEpoll();
  int client_socket_;
  int ep_fd_;
};

#endif
