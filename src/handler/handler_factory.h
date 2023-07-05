#ifndef __HANDLER_FACTORY_H_
#define __HANDLER_FACTORY_H_

#include <memory>

#include "handler/base_handler.h"
#include "handler/login_handler.h"
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
