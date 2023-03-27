#include "test/xgt_test_trader.h"
#include "interface/logger.h"
#include "unordered_map"
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>

void test_func() {
  std::srand(std::time(nullptr));
  for (int i = 0; i < 1000; ++i) {
    int random = std::rand() % 6 + 1;
    switch (random) {
    case 1:
      LOG_DEBUG("Test Debug: thread_id is: %d, Test function", std::this_thread::get_id());
      break;
    case 2:
      LOG_INFO("Test Info: %d.", random);
      break;
    case 3:
      LOG_WARN("Test WARN: %d.", random);
      break;
    case 4:
      LOG_ERROR("Test Error: %d.", random);
      break;
    case 5:
      LOG_FATAL("Test Fatal: %d.", random);
      break;
    case 6:
      LOG_SUCCESS("Test Success: %d.", random);
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main(int argc, char** argv) {
  std::string binary_path{argv[0]};
  std::stringstream ss(binary_path);
  std::string item;
  while (getline(ss, item, '/')) {
  }
  std::cout << item << std::endl;
  XGT::Logger::Init(item, true, "./log", 500);
  std::vector<std::thread> thread_vec;
  for (int i = 0; i < 3; ++i) {
    thread_vec.push_back(std::thread(test_func));
  }
  for (int i = 0; i < 3; ++i) {
    thread_vec[i].join();
  }
  return 0;
}
