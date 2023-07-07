#include "util/timer_fd.h"

TimerFd::TimerFd() {
  timer_fd_ = timerfd_create(CLOCK_REALTIME, 0);
}

bool TimerFd::SetTimeout(int second, uint64_t nanosecond) {
  struct timespec now;
  if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
    perror("clock get time error");
    return false;
  }
  struct itimerspec new_value;
  new_value.it_value.tv_sec = now.tv_sec;
  new_value.it_value.tv_nsec = now.tv_nsec;
  new_value.it_interval.tv_sec = second;
  new_value.it_interval.tv_nsec = nanosecond;
  if (timerfd_settime(timer_fd_, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) {
    perror("Set Time Error");
    return false;
  }
  return true;
}

int TimerFd::GetTimerFd() {
  return timer_fd_;
}
