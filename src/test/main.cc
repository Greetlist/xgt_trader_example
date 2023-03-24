#include "test/xgt_test_trader.h"
#include "unordered_map"
#include <chrono>
#include <string>

int main(int argc, char** argv) {
  std::unordered_map<std::string, std::string> config {
    {"account", "test"},
    {"password", "testpassword"},
    {"op_station", "testopstation"},
  };
  TestTrader* trader = new TestTrader(std::move(config));
  trader->Init();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  for (int i = 0; i < 5; ++i) {
    trader->SubscribeTopic();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    trader->QueryAccount();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    trader->QueryPosition();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    trader->QueryOrder();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    trader->QueryTrade();
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  return 0;
}
