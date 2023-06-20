#include "test_client/xgt_test_trader.h"
#include "logger/logger.h"
#include "unordered_map"
#include <thread>
#include <chrono>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char** argv) {
  Logger::Init(argv[0], true, "./log");
  std::vector<std::thread> tv;
  int thread_num = 20;
  int req_num = 20000;
  int loop_count = 20;

  for (int i = 0; i < thread_num; ++i) {
    tv.emplace_back(std::move([&]() {
      std::unordered_map<std::string, std::string> config {
        {"server", "127.0.0.1"},
        {"port", "10000"},
        {"account", "test"},
        {"fund_account", "BH300214681"},
        {"password", "passwd"},
        {"op_station", "test_op_station"},
      };
      TestTrader* trader = new TestTrader(std::move(config));
      int res = trader->Init();
      LOG_INFO("res is: %d", res);
      for (int l = 0; l < loop_count; ++l) {
        for (int r = 0; r < req_num; ++r) {
          trader->SubscribeTopic();
          trader->InsertOrder();
          trader->CancelOrder();
          trader->QueryAccount();
          trader->QueryPosition();
          trader->QueryOrder();
          trader->QueryTrade();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
      delete trader;
    }));
  }

  for (int i = 0; i < thread_num; ++i) {
    tv[i].join();
  }
  return 0;
}
