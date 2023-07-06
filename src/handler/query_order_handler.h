#ifndef __QUERY_ORDER_HANDLER_H_
#define __QUERY_ORDER_HANDLER_H_

#include "handler/base_handler.h"

class QueryOrderHandler : public BaseHandler {
public:
  explicit QueryOrderHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~QueryOrderHandler() = default;
  virtual void HandleRequest() override;
};

#endif
