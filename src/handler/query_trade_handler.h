#ifndef __QUERY_TRADE_HANDLER_H_
#define __QUERY_TRADE_HANDLER_H_

#include "handler/base_handler.h"

class QueryTradeHandler : public BaseHandler {
public:
  explicit QueryTradeHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~QueryTradeHandler() = default;
  virtual void HandleRequest() override;
};

#endif
