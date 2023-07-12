#include "handler/login_handler.h"
#include "logger/logger.h"

void LoginHandler::HandleRequest() {
  LOG_INFO("LoginHandler::HandleRequest");
  XGT::XGTLoginRequest& req = req_.login_req;
  LOG_INFO("%s, %s, %s", req.account, req.password, req.op_station);
}

bool LoginHandler::PasswordValid() {
  return true;
}
