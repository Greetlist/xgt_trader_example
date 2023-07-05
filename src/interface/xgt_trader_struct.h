#ifndef __XGT_TRADER_STRUCT_H_
#define __XGT_TRADER_STRUCT_H_

#pragma pack(push, 1)

#include <sys/types.h>
#include <string.h>
#include "interface/xgt_trader_enum.h"

namespace XGT {

struct XGTLoginRequest {
  char account[32];
  char password[32];
  char op_station[256];
  XGTLoginRequest() {
    memset(account, 0, 32);
    memset(password, 0, 32);
    memset(op_station, 0, 32);
  }
};

struct XGTLoginResponse {
  char sh_fund_account[32];
  char sz_fund_account[32];
  int return_code;
  char msg[32];
  XGTLoginResponse() {
    memset(sh_fund_account, 0, 32);
    memset(sz_fund_account, 0, 32);
    return_code = 0;
    memset(msg, 0, 32);
  }
};

struct XGTLogoutRequest {
  char account[32];
  XGTLogoutRequest() {
    memset(account, 0, 32);
  }
};

struct XGTSubscribeRequest {
  SubscribeChannel channel;
  XGTSubscribeRequest() {
    channel = SubscribeChannel::Unknown;
  }
};

struct XGTSubscribeTopicResponse {
  int return_code;
  char msg[32];
  XGTSubscribeTopicResponse() {
    return_code = 0;
    memset(msg, 0, 32);
  }
};

struct XGTInsertOrderRequest {
  double price;
  XGT::Exchange exchange_id;
  char instrument_id[32];
  char fund_account[32];
  char account[32];
  int volume;
  OrderSide side;
  OrderType type;
  XGTInsertOrderRequest() {
    price = 0.0;
    exchange_id = Exchange::ALL;
    memset(instrument_id, 0, 32);
    memset(fund_account, 0, 32);
    memset(account, 0, 32);
    volume = 0;
    side = OrderSide::Unknown;
    type = OrderType::Unknown;
  }
};

struct XGTReturnOrder {
  double price;
  char order_sys_id[64];
  int return_code;
  char msg[32];
  int order_volume;
  int traded_volume;
  int cancel_volume;
  char instrument_id[32];
  XGT::Exchange exchange_id;
  OrderSide side;
  OrderStatus order_status;
  XGTReturnOrder() {
  }
};

struct XGTReturnTrade {
  double price;
  char order_sys_id[64];
  int return_code;
  char msg[32];
  int order_volume;
  int traded_volume;
  int cancel_volume;
  char instrument_id[32];
  XGT::Exchange exchange_id;
  OrderSide side;
};

struct XGTCancelOrderRequest {
  char order_sys_id[64];
  XGT::Exchange exchange_id;
};

struct XGTReturnAction {
  char order_sys_id[64];
  int return_code;
  char msg[32];
  OrderStatus order_status;
};

struct XGTQueryAccountRequest {
  char fund_account[32];
  char account[32];
};

struct XGTQueryPositionRequest {
  char fund_account[32];
  char account[32];
  Exchange exchange_id;
};

struct XGTQueryOrderRequest {
  char fund_account[32];
  char account[32];
};

struct XGTQueryTradeRequest {
  char fund_account[32];
  char account[32];
};

struct XGTAccountInfo {
  double total_assets;
  double enable_balance;
  double withdraw_balance;
  double frozen_balance;
  double init_balance;
};

struct XGTPositionInfo {
  char instrument_id[32];
  XGT::Exchange exchange_id;
  int current_volume;
  int yest_init_volume;
  int today_buy_volume;
  int today_sell_volume;
};

struct XGTOrderInfo {
  char order_sys_id[64];
  char instrument_id[32];
  XGT::Exchange exchange_id;
  int volume;
  int traded_volume;
  double order_price;
  int64_t order_time;
  OrderStatus order_status;
  OrderSide side;
};

struct XGTTradeInfo {
  char order_sys_id[64];
  char instrument_id[32];
  XGT::Exchange exchange_id;
  int traded_volume;
  double traded_price;
  int64_t trade_time;
  OrderSide side;
};

struct XGTQryPositionResponse {
  char instrument_id[32];
  XGT::Exchange exchange_id;
  int volume;
};

typedef union _XGTRequest {
  XGTLoginRequest login_req;
  XGTLogoutRequest logout_req;
  XGTSubscribeRequest subscribe_req;
  XGTInsertOrderRequest insert_req;
  XGTCancelOrderRequest cancel_req;
  XGTQueryAccountRequest query_account_req;
  XGTQueryPositionRequest query_position_req;
  XGTQueryOrderRequest query_order_req;
  XGTQueryTradeRequest query_trade_req;
} XGTRequest;

typedef union _XGTResponse {
  XGTLoginResponse login_res;
  XGTSubscribeTopicResponse subscribe_res;
  XGTReturnOrder return_order;
  XGTReturnTrade return_trade;
  XGTReturnAction cancel_res;
  XGTQryPositionResponse position_res;
} XGTResponse;

#pragma pack(pop)

}//namespace XGT

#endif
