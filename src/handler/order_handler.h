#ifndef __ORDER_HANDLER_H_
#define __ORDER_HANDLER_H_

#include "handler/base_handler.h"

class OrderHandler : public BaseHandler {
public:
  explicit OrderHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~OrderHandler() = default;
  virtual void HandleRequest() override;
};

#endif
