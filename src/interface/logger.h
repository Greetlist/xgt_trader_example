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

//template <LOGLEVEL L, char* FILE=__FILE__, int LINE=__LINE__>
template <LOGLEVEL L, int LINE=__LINE__>
static void XLOG(const char* fmt, ...);

int GenTimeString(char* buf, size_t len, const char* format_template);

class Logger {
public:
  static void Init(const std::string& binary_name, const std::string& base_dir="./log", const int max_log_line=100000);
  Logger();
  ~Logger();
  Logger(const Logger&) = delete;
private:
  inline static Logger& GetInstance() {
    static Logger LogInstance;
    return LogInstance;
  }
  class InnerFileConfig {
  public:
    int current_log_line_;
    std::fstream file_stream_;
    std::vector<std::string>* log_cache_;
    std::mutex cache_mutex_;
  };
  static std::string log_basedir_;
  static std::string binary_name_;
  static int max_log_line_;
  std::fstream NewFstream(int level);
  void WriteLogToFile();
  InnerFileConfig* NewInnerConfigs(int level);
  std::unordered_map<int, InnerFileConfig*> configs_;
  std::thread log_writer;
};

//#define LOG_DEBUG(fmt, ...) LOG(DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
//#define LOG_INFO(fmt, ...) LOG(INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
//#define LOG_WARN(fmt, ...) LOG(WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
//#define LOG_ERROR(fmt, ...) LOG(ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
//#define LOG_FATAL(fmt, ...) LOG(FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
//#define LOG_SUCCESS(fmt, ...) LOG(SUCCESS, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

} //namespace XGT

#endif
