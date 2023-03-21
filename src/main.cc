#include <iostream>
#include "interface/epoll_static.h"

int main(int argc, char** argv)
{
  EpollInstance* inst = EpollInstance::GetInstance();
  std::cout << "epoll_fd: " << inst->ep_fd_ << std::endl;
  return 0;
}
