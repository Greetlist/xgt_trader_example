#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <type_traits>
#include <vector>
#include <string>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <fstream>
#include <chrono>
#include <iostream>
#include <atomic>

namespace XGT {

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

static constexpr int COLOR_LEN = strlen(RED);
static constexpr int RESET_LEN = strlen(RESET);

//static char LOGTEXT[][16] = {"", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "SUCCESS"};
//static char LOGCOLOR[][16] = {RESET, MAGENTA, BLUE, YELLOW, RED, RED, GREEN};

enum LOGLEVEL {
  NONE = 0,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
  SUCCESS,
  NUM
};

static std::vector<std::string> LOGTEXT {
  "",
  "DEBUG",
  "INFO",
  "WARN",
  "ERROR",
  "FATAL",
  "SUCCESS",
};

static std::vector<std::string> LOGCOLOR{RESET, MAGENTA, BLUE, YELLOW, RED, RED, GREEN};

int GenTimeString(char* buf, size_t len, const char* format_template);

class Logger {
public:
  static void Init(const std::string& binary_name, bool log_to_std=false, const std::string& base_dir="./log", const int max_log_line=100000);
  static void LOG(LOGLEVEL L, char* FILE, int LINE, const char* fmt, ...);
  Logger();
  ~Logger();
  Logger(const Logger&) = delete;
private:
  inline static Logger& GetInstance() {
    static Logger LogInstance;
    return LogInstance;
  }
  void StartWriterThread();
  class InnerFileConfig {
  public:
    int current_log_line_;
    std::fstream file_stream_;
    std::vector<std::string>* log_cache_;
    std::mutex cache_mutex_;
  };
  static std::string log_basedir_;
  static bool log_to_std_;
  static std::string binary_name_;
  static int max_log_line_;
  std::fstream NewFstream(int level);
  void WriteLogToFile();
  InnerFileConfig* NewInnerConfigs(int level);
  std::unordered_map<int, InnerFileConfig*> configs_;
  std::thread log_writer;
  std::atomic<bool> stop_{false};
};

#define LOG_DEBUG(fmt, ...) XGT::Logger::LOG(XGT::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) XGT::Logger::LOG(XGT::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) XGT::Logger::LOG(XGT::WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) XGT::Logger::LOG(XGT::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) XGT::Logger::LOG(XGT::FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_SUCCESS(fmt, ...) XGT::Logger::LOG(XGT::SUCCESS, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

} //namespace XGT

#endif
