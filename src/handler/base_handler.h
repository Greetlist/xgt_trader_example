#ifndef __BASE_HANDLER_H_
#define __BASE_HANDLER_H_

#include "interface/xgt_trader_struct.h"

class BaseHandler {
public:
  explicit BaseHandler(const XGT::XGTRequest& req) : req_(req) {}
  virtual ~BaseHandler() {}
  virtual void HandleRequest() {}
  XGT::XGTRequest req_;
};

#endif
