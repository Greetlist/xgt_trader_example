#ifndef __QUERY_ACCOUNT_HANDLER_H_
#define __QUERY_ACCOUNT_HANDLER_H_

#include "handler/base_handler.h"

class QueryAccountHandler : public BaseHandler {
public:
  explicit QueryAccountHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~QueryAccountHandler() = default;
  virtual void HandleRequest() override;
};

#endif
