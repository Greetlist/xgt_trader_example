#ifndef __XGT_TRADER_EMU_H_
#define __XGT_TRADER_EMU_H_

namespace XGT {

enum RequestType {
  LoginRequest = 1,
  LogoutRequest,
  SubscribeRequest,
  InsertOrderRequest,
  CancelOrderRequest,
  QueryAccountRequest,
  QueryPositionRequest,
  QueryOrderRequest,
  QueryTradeRequest,
  HeartbeatRequest,
  UnknownRequest,
};

enum ResponseType {
  LoginResponse = 1,
  LogoutResponse,
  SubscribeResponse,
  ReturnOrder,
  ReturnTrade,
  QueryAccountResponse,
  QueryPositionResponse,
  QueryOrderResponse,
  QueryTradeResponse,
  UnknownResponse,
};

enum SubscribeChannel {
  Order = 1,
  Trade = 2,
  UnknownChannel,
};

enum OrderSide {
  Buy = 1,
  Sell = 2,
  UnknownOrderSide,
};

enum OrderStatus {
  Notified = 1,
  Accepted = 2,
  PartTraded = 3,
  AllTraded = 4,
  Rejected = 5,
  Canceling = 6,
  CancelDone = 7,
  CancelFailed = 8,
  UnknownOrderStatus,
};

enum OrderType {
  MarketPrice = 1,
  LimitPrice = 2,
  UnknownOrderType,
};

enum Exchange {
  SH = 1,
  SZ = 2,
  ALL = 3,
  UnknownExchange,
};

}// namespace XGT

#endif
