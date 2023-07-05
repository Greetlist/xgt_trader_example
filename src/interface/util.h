#ifndef __UTIL_H_
#define __UTIL_H_

#include "interface/xgt_trader_struct.h"

namespace XGT {

template <typename T>
int GetRequestType(const T& req) {
  if constexpr (std::is_same<T, XGTLoginRequest>::value) {
    return RequestType::LoginRequest;
  } else if constexpr (std::is_same<T, XGTLogoutRequest>::value) {
    return RequestType::LogoutRequest;
  } else if constexpr (std::is_same<T, XGTSubscribeRequest>::value) {
    return RequestType::SubscribeRequest;
  } else if constexpr (std::is_same<T, XGTInsertOrderRequest>::value) {
    return RequestType::InsertOrderRequest;
  } else if constexpr (std::is_same<T, XGTCancelOrderRequest>::value) {
    return RequestType::CancelOrderRequest;
  } else if constexpr (std::is_same<T, XGTQueryAccountRequest>::value) {
    return RequestType::QueryAccountRequest;
  } else if constexpr (std::is_same<T, XGTQueryPositionRequest>::value) {
    return RequestType::QueryPositionRequest;
  } else if constexpr (std::is_same<T, XGTQueryOrderRequest>::value) {
    return RequestType::QueryOrderRequest;
  } else if constexpr (std::is_same<T, XGTQueryTradeRequest>::value) {
    return RequestType::QueryTradeRequest;
  }
  return RequestType::UnknownRequest;
}

} //namespace XGT

#endif
