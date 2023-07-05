#ifndef __MESSAGE_CODER_H_
#define __MESSAGE_CODER_H_

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "interface/xgt_trader_struct.h"

class MessageCoder {
public:
  MessageCoder() = delete;
  ~MessageCoder();
  template <class T>
  static nlohmann::json StructToJson(const T& s) {
    nlohmann::json res;
    if constexpr (std::is_same<T, XGT::XGTLoginRequest>::value) {
      res["account"] = s.account;
      res["password"] = s.password;
      res["op_station"] = s.op_station;
    } else if constexpr (std::is_same<T, XGT::XGTLogoutRequest>::value) {
      res["account"] = s.account;
    } else if constexpr (std::is_same<T, XGT::XGTSubscribeRequest>::value) {
      res["channel"] = s.channel;
    } else if constexpr (std::is_same<T, XGT::XGTInsertOrderRequest>::value) {
      res["price"] = s.price;
      res["exchange_id"] = s.exchange_id;
      res["instrument_id"] = s.instrument_id;
      res["fund_account"] = s.fund_account;
      res["account"] = s.account;
      res["volume"] = s.volume;
      res["side"] = s.side;
      res["type"] = s.type;
    } else if constexpr (std::is_same<T, XGT::XGTCancelOrderRequest>::value) {
      res["order_sys_id"] = s.order_sys_id;
      res["exchange_id"] = s.exchange_id;
    } else if constexpr (std::is_same<T, XGT::XGTQueryAccountRequest>::value) {
      res["fund_account"] = s.fund_account;
      res["account"] = s.account;
    } else if constexpr (std::is_same<T, XGT::XGTQueryPositionRequest>::value) {
      res["fund_account"] = s.fund_account;
      res["account"] = s.account;
      res["exchange_id"] = s.exchange_id;
    } else if constexpr (std::is_same<T, XGT::XGTQueryOrderRequest>::value) {
      res["fund_account"] = s.fund_account;
      res["account"] = s.account;
    } else if constexpr (std::is_same<T, XGT::XGTQueryTradeRequest>::value) {
      res["fund_account"] = s.fund_account;
      res["account"] = s.account;
    } else if constexpr (std::is_same<T, XGT::XGTSubscribeTopicResponse>::value) {
      res["return_code"] = s.return_code;
      res["msg"] = s.msg;
    } else if constexpr (std::is_same<T, XGT::XGTLoginResponse>::value) {
      res["sh_fund_account"] = s.sh_fund_account;
      res["sz_fund_account"] = s.sz_fund_account;
      res["return_code"] = s.return_code;
      res["msg"] = s.msg;
    }
    return res;
  }

  static XGT::XGTRequest JsonToRequest(int request_type, const nlohmann::json& json) {
    XGT::XGTRequest req;
    switch (request_type) {
    case XGT::LoginRequest:
      Convert2Request(json, req.login_req);
      break;
    case XGT::LogoutRequest:
      Convert2Request(json, req.logout_req);
      break;
    case XGT::SubscribeRequest:
      Convert2Request(json, req.subscribe_req);
      break;
    case XGT::InsertOrderRequest:
      Convert2Request(json, req.insert_req);
      break;
    case XGT::CancelOrderRequest:
      Convert2Request(json, req.cancel_req);
      break;
    case XGT::QueryAccountRequest:
      Convert2Request(json, req.query_account_req);
      break;
    case XGT::QueryPositionRequest:
      Convert2Request(json, req.query_position_req);
      break;
    case XGT::QueryOrderRequest:
      Convert2Request(json, req.query_order_req);
      break;
    case XGT::QueryTradeRequest:
      Convert2Request(json, req.query_trade_req);
      break;
    }
    return req;
  }

  template <class T>
  static void Convert2Request(const nlohmann::json& json, T& req) {
    if constexpr (std::is_same<T, XGT::XGTLoginRequest>::value) {
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
      memcpy(&req.password, json["password"].get<std::string>().c_str(), 32);
      memcpy(&req.op_station, json["op_station"].get<std::string>().c_str(), 256);
    } else if constexpr (std::is_same<T, XGT::XGTLogoutRequest>::value) {
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
    } else if constexpr (std::is_same<T, XGT::XGTSubscribeRequest>::value) {
      req.channel = static_cast<XGT::SubscribeChannel>(json["channel"].get<int>());
    } else if constexpr (std::is_same<T, XGT::XGTInsertOrderRequest>::value) {
      req.price = json["price"].get<double>();
      req.exchange_id = static_cast<XGT::Exchange>(json["exchange_id"].get<int>());
      memcpy(&req.instrument_id, json["instrument_id"].get<std::string>().c_str(), 32);
      memcpy(&req.fund_account, json["fund_account"].get<std::string>().c_str(), 32);
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
      req.volume = json["volume"].get<int>();
      req.side = static_cast<XGT::OrderSide>(json["side"].get<int>());
      req.type = static_cast<XGT::OrderType>(json["type"].get<int>());
    } else if constexpr (std::is_same<T, XGT::XGTCancelOrderRequest>::value) {
      memcpy(&req.order_sys_id, json["order_sys_id"].get<std::string>().c_str(), 64);
      memcpy(&req.exchange_id, json["exchange_id"].get<std::string>().c_str(), 32);
    } else if constexpr (std::is_same<T, XGT::XGTQueryAccountRequest>::value) {
      memcpy(&req.account, json["fund_account"].get<std::string>().c_str(), 32);
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
    } else if constexpr (std::is_same<T, XGT::XGTQueryPositionRequest>::value) {
      memcpy(&req.account, json["fund_account"].get<std::string>().c_str(), 32);
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
      req.exchange_id = static_cast<XGT::Exchange>(json["exchange_id"].get<int>());
    } else if constexpr (std::is_same<T, XGT::XGTQueryOrderRequest>::value) {
      memcpy(&req.account, json["fund_account"].get<std::string>().c_str(), 32);
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
    } else if constexpr (std::is_same<T, XGT::XGTQueryTradeRequest>::value) {
      memcpy(&req.account, json["fund_account"].get<std::string>().c_str(), 32);
      memcpy(&req.account, json["account"].get<std::string>().c_str(), 32);
    }
  }

  static XGT::XGTResponse JsonToResponse(int response_type, const nlohmann::json& json) {
    XGT::XGTResponse res;
    switch (response_type) {
    case XGT::LoginResponse:
      Convert2Response(json, res.login_res);
      break;
    case XGT::SubscribeResponse:
      Convert2Response(json, res.subscribe_res);
      break;
    }
    return res;
  }

  template <class T>
  static void Convert2Response(const nlohmann::json& json, T& res) {
    if constexpr (std::is_same<T, XGT::XGTLoginResponse>::value) {
      memcpy(&res.sh_fund_account, json["sh_fund_account"].get<std::string>().c_str(), 32);
      memcpy(&res.sz_fund_account, json["sz_fund_account"].get<std::string>().c_str(), 32);
      res.return_code = json["return_code"].get<int>();
      memcpy(&res.msg, json["msg"].get<std::string>().c_str(), 32);
    } else if constexpr (std::is_same<T, XGT::XGTSubscribeTopicResponse>::value) {
      res.return_code = json["return_code"].get<int>();
      memcpy(&res.msg, json["msg"].get<std::string>().c_str(), 32);
    }
  }
};

#endif
