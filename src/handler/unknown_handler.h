#ifndef __UNKNOWN_HANDLER_H_
#define __UNKNOWN_HANDLER_H_

#include "handler/base_handler.h"

class UnknownHandler : public BaseHandler {
public:
  explicit UnknownHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~UnknownHandler() = default;
  virtual void HandleRequest() override;
};

#endif
