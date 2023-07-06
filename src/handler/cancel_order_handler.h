#ifndef __CANCEL_ORDER_HANDLER_H_
#define __CANCEL_ORDER_HANDLER_H_

#include "handler/base_handler.h"

class CancelOrderHandler : public BaseHandler {
public:
  explicit CancelOrderHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~CancelOrderHandler() = default;
  virtual void HandleRequest() override;
};

#endif
