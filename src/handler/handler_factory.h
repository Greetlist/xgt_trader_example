#ifndef __HANDLER_FACTORY_H_
#define __HANDLER_FACTORY_H_

#include <memory>

#include "handler/base_handler.h"
#include "handler/login_handler.h"
#include "handler/logout_handler.h"
#include "handler/cancel_order_handler.h"
#include "handler/order_handler.h"
#include "handler/query_account_handler.h"
#include "handler/query_position_handler.h"
#include "handler/query_order_handler.h"
#include "handler/query_trade_handler.h"
#include "handler/subscribe_handler.h"
#include "handler/unknown_handler.h"

#include "interface/xgt_trader_enum.h"

class HandlerFactory {
public:
  ~HandlerFactory() = default;
  static std::unique_ptr<BaseHandler> GetHandler(int request_type, const XGT::XGTRequest& req) {
    std::unique_ptr<BaseHandler> res;
    switch (request_type) {
    case XGT::RequestType::LoginRequest:
      res = std::make_unique<LoginHandler>(req);
      break;
    case XGT::RequestType::LogoutRequest:
      res = std::make_unique<LogoutHandler>(req);
      break;
    case XGT::RequestType::SubscribeRequest:
      res = std::make_unique<SubscribeHandler>(req);
      break;
    case XGT::RequestType::InsertOrderRequest:
      res = std::make_unique<OrderHandler>(req);
      break;
    case XGT::RequestType::CancelOrderRequest:
      res = std::make_unique<CancelOrderHandler>(req);
      break;
    case XGT::RequestType::QueryAccountRequest:
      res = std::make_unique<QueryAccountHandler>(req);
      break;
    case XGT::RequestType::QueryPositionRequest:
      res = std::make_unique<QueryPositionHandler>(req);
      break;
    case XGT::RequestType::QueryOrderRequest:
      res = std::make_unique<QueryOrderHandler>(req);
      break;
    case XGT::RequestType::QueryTradeRequest:
      res = std::make_unique<QueryTradeHandler>(req);
      break;
    default:
      res = std::make_unique<UnknownHandler>(req);
      break;
    }
    return std::move(res);
  }
private:
  HandlerFactory();
};

#endif
