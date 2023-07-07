#ifndef __TIMER_FD_H_
#define __TIMER_FD_H_

#include <sys/timerfd.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>

class TimerFd {
public:
  TimerFd();
  ~TimerFd() = default;
  bool SetTimeout(int, uint64_t); //first unit: second, second unit: nanosecond
  int GetTimerFd();
private:
  int timer_fd_;
};

#endif
