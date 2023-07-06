#ifndef __QUERY_POSITION_HANDLER_H_
#define __QUERY_POSITION_HANDLER_H_

#include "handler/base_handler.h"

class QueryPositionHandler : public BaseHandler {
public:
  explicit QueryPositionHandler(const XGT::XGTRequest& req) : BaseHandler(req) {}
  ~QueryPositionHandler() = default;
  virtual void HandleRequest() override;
};

#endif
