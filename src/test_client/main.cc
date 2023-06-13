#include "test/xgt_test_trader.h"
#include "interface/logger.h"
#include "unordered_map"
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include <thread>

int main(int argc, char** argv) {
  std::string binary_path{argv[0]};
  std::stringstream ss(binary_path);
  std::string item;
  while (getline(ss, item, '/')) {
  }
  std::unordered_map<std::string, std::string> config {
    {"server", "127.0.0.1"},
    {"port", "10000"},
    {"account", "test"},
    {"fund_account", "BH300214681"},
    {"password", "passwd"},
    {"op_station", "test_op_station"},
  };
  XGT::Logger::Init(item, true, "./log", 500);
  TestTrader* trader = new TestTrader(std::move(config));
  int res = trader->Init();
  LOG_INFO("res is: %d", res);
  for (int i = 0; i < 2; ++i) {
    trader->SubscribeTopic();
    trader->InsertOrder();
    trader->CancelOrder();
    trader->QueryAccount();
    trader->QueryPosition();
    trader->QueryOrder();
    trader->QueryTrade();
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  delete trader;
  return 0;
}
