#ifndef __XGT_TRADER_STRUCT_H_
#define __XGT_TRADER_STRUCT_H_

#include <memory>

#include "interface/xgt_trader_emu.h"

namespace XGT {

struct XGTLoginRequest {
  char account[32];
  char password[32];
  char op_station[128];
};

struct XGTLoginResponse {
  char sh_fund_account[32];
  char sz_fund_account[32];
  int return_code;
  char msg[32];
};

struct XGTLogouRequest {
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
  char instrument_id[32];
  char exchange_id[32];
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
  int volume;
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
  int volume;
  int traded_volume;
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
  int volume;
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

}//namespace XGT

#endif
