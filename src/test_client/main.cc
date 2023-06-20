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
#include <atomic>
#include <vector>

int main(int argc, char** argv) {
  Logger::Init(argv[0], true, "./log", 50000);
  std::vector<std::thread> tv;
  int thread_num = 20;
  int run_seconds = 300;
  std::atomic<bool> stop{false};

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
      while (!stop) {
        trader->SubscribeTopic();
        trader->InsertOrder();
        trader->CancelOrder();
        trader->QueryAccount();
        trader->QueryPosition();
        trader->QueryOrder();
        trader->QueryTrade();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      delete trader;
    }));
  }

  for (int i = 0; i < run_seconds; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  stop.store(true);

  for (int i = 0; i < thread_num; ++i) {
    tv[i].join();
  }
  return 0;
}
