#ifndef __XGT_TRADER_STRUCT_H_
#define __XGT_TRADER_STRUCT_H_

#pragma pack(push, 1)

#include <sys/types.h>
#include "interface/xgt_trader_enum.h"

namespace XGT {

struct XGTLoginRequest {
  char account[32];
  char password[32];
  char op_station[256];
};

struct XGTLoginResponse {
  char sh_fund_account[32];
  char sz_fund_account[32];
  int return_code;
  char msg[32];
};

struct XGTLogoutRequest {
  char account[32];
};

struct XGTSubscribeRequest {
  SubscribeChannel channel;
};

struct XGTSubscribeTopicResponse {
  int return_code;
  char msg[32];
};

struct XGTInsertOrderRequest {
  double price;
  char instrument_id[16];
  char exchange_id[16];
  char fund_account[32];
  char account[32];
  int volume;
  OrderSide side;
  OrderType type;
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
  char exchange_id[32];
  OrderSide side;
  OrderStatus order_status;
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
  char exchange_id[32];
  OrderSide side;
};

struct XGTCancelOrderRequest {
  char order_sys_id[64];
  char exchange_id[32];
};

struct XGTReturnAction {
  char order_sys_id[64];
  int return_code;
  char msg[32];
  OrderStatus order_status;
};

struct XGTQueryCommonRequest {
  char fund_account[32];
  char account[32];
  RequestType request_type;
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
  char exchange_id[32];
  int current_volume;
  int yest_init_volume;
  int today_buy_volume;
  int today_sell_volume;
};

struct XGTOrderInfo {
  char order_sys_id[64];
  char instrument_id[32];
  char exchange_id[32];
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
  char exchange_id[32];
  int traded_volume;
  double traded_price;
  int64_t trade_time;
  OrderSide side;
};

struct XGTQryPositionResponse {
  char instrument_id[32];
  char exchange_id[32];
  int volume;
};

typedef union _XGTRequest {
  XGTLoginRequest login_req;
  XGTLogoutRequest logout_req;
  XGTSubscribeRequest subscribe_req;
  XGTInsertOrderRequest insert_req;
  XGTCancelOrderRequest cancel_req;
  XGTQueryCommonRequest query_req;
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
