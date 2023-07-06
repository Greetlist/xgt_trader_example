#ifndef __LOGOUT_HANDLER_H_
#define __LOGOUT_HANDLER_H_

#include "handler/base_handler.h"

class LogoutHandler : public BaseHandler {
public:
  explicit LogoutHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~LogoutHandler() = default;
  virtual void HandleRequest() override;
};

#endif
