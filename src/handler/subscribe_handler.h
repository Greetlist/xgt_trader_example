#ifndef __SUBSCRIBE_HANDLER_H_
#define __SUBSCRIBE_HANDLER_H_

#include "handler/base_handler.h"

class SubscribeHandler : public BaseHandler {
public:
  explicit SubscribeHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~SubscribeHandler() = default;
  virtual void HandleRequest() override;
};

#endif
