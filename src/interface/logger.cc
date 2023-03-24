#include "interface/logger.h"

namespace XGT {

std::string Logger::log_basedir_;
std::string Logger::binary_name_;
int Logger::max_log_line_;

int GenTimeString(char* buf, size_t len, const char* format_template) {
  memset(buf, 0, len);
  struct timeval t_val;
  gettimeofday(&t_val, NULL);

  struct tm t;
  strftime(buf, len, format_template, localtime_r(&t_val.tv_sec, &t));
  return sprintf(buf, "%s.%ld", buf, t_val.tv_usec);
}

//template <LOGLEVEL L, char* FILE=__FILE__, int LINE=__LINE__>
template <LOGLEVEL L, int LINE=__LINE__>
void LOG(const char* fmt, ...) {
  if (L < LOGLEVEL::DEBUG) {
    return;
  }

  int fmt_str_len;
  char *p = NULL;
  va_list v_list;

  va_start(v_list, fmt);
  fmt_str_len = vsnprintf(p, fmt_str_len, fmt, v_list);
  va_end(v_list);
  fmt_str_len++;  /* For '\0' */
   
  char fmt_str[fmt_str_len];
  va_start(v_list, fmt);
  vsnprintf(fmt_str, fmt_str_len, fmt, v_list);
  va_end(v_list);

  char time_str[128];
  int time_len = GenTimeString(time_str, 128, "%Y-%m-%d %H:%M:%S");

  int big_enough_len = fmt_str_len + 256;
  if (big_enough_len > 65535) {
    big_enough_len = 65535;
  }
  char final_str[big_enough_len];
  int total_len = sprintf(final_str, "%s[%s][%s][%s:%d] %s%s", LOGCOLOR[L], LOGTEXT[L], time_str, __FILE__, LINE, fmt_str, LOGCOLOR[0]);
  std::string res(final_str);
  Logger& instance = Logger::GetInstance();
  instance.configs_[L]->log_cache_->push_back(std::move(res));
}

void Logger::Init(const std::string& binary_name, const std::string& base_dir, const int max_log_line) {
  Logger::log_basedir_ = base_dir;
  Logger::binary_name_ = binary_name;
  Logger::max_log_line_ = max_log_line;
  Logger::GetInstance(); //start thread
}

Logger::Logger() {
  for (int i = 1; i < LOGLEVEL::NUM; ++i) {
    configs_[i] = NewInnerConfigs(i);
  }
  log_writer = std::thread(&Logger::WriteLogToFile, this);
}

Logger::~Logger() {
  for (int i = 1; i < LOGLEVEL::NUM; ++i) {
    delete configs_[i];
  }
}

void Logger::WriteLogToFile() {
  for (int i = 1; i < LOGLEVEL::NUM; ++i) {
    configs_[i]->cache_mutex_.lock();
    std::vector<std::string> cache_copy = std::move(*configs_[i]->log_cache_);
    configs_[i]->cache_mutex_.unlock();
    int size = cache_copy.size();
    if (configs_[i]->current_log_line_ + size > Logger::max_log_line_) {
      int left = Logger::max_log_line_ - configs_[i]->current_log_line_;
      for (int i = 0; i < left; ++i) {
        configs_[i]->file_stream_ << cache_copy[i] << "\n";
      }
      configs_[i]->file_stream_.flush();
      configs_[i]->file_stream_.close();
      configs_[i]->file_stream_ = NewFstream(i);
      for (int i = left; i < size - left; ++i) {
        configs_[i]->file_stream_ << cache_copy[i] << "\n";
      }
    } else {
      for (const std::string& s : cache_copy) {
        configs_[i]->file_stream_ << s << "\n";
      }
      configs_[i]->current_log_line_ += size;
    }
  }
  std::this_thread::sleep_for(std::chrono::seconds(5));
}

std::fstream Logger::NewFstream(int level) {
  char time_str[128];
  GenTimeString(time_str, 128, "%Y-%m-%d.%H:%M:%S");
  std::string file_name = Logger::log_basedir_ + "/" + Logger::binary_name_ + "." + std::string{time_str} + "." + LOGTEXT[level];
  std::fstream file_stream = std::fstream(file_name, std::ios_base::app | std::ios_base::in | std::ios_base::out);
  return file_stream;
}

Logger::InnerFileConfig* Logger::NewInnerConfigs(int level) {
  InnerFileConfig* res = new InnerFileConfig();
  res->current_log_line_ = 0;
  res->file_stream_ = NewFstream(level);
  res->log_cache_ = new std::vector<std::string>();
  return res;
}

} //namespace XGT
