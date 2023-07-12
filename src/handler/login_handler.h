#ifndef __LOGIN_HANDLER_H_
#define __LOGIN_HANDLER_H_

#include "handler/base_handler.h"

class LoginHandler : public BaseHandler {
public:
  explicit LoginHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~LoginHandler() = default;
  virtual void HandleRequest() override;
  bool PasswordValid();
};

#endif
