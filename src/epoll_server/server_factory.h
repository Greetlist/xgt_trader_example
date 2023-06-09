#ifndef __SERVER_FACTORY_H_
#define __SERVER_FACTORY_H_

#include <csignal>
#include <iostream>

#include "epoll_server/epoll_tcp_server.h"
#include "epoll_server/epoll_udp_server.h"
#include "epoll_server/global_def.h"

class EpollServerFactory {
 public:
   EpollServerFactory() = delete;
   ~EpollServerFactory();
   static EpollServerBase* GetServer(const ServerType& server_type, const EpollTriggerMode& trigger_mode=EpollTriggerMode::ET, const std::string& listen_addr="0.0.0.0", const int& listen_port=1000, const int& parallel=5, const int& msg_processor_num=20) {
     EpollServerFactory::InitServer(server_type, trigger_mode, listen_addr, listen_port, parallel, msg_processor_num);
     return instance_;
   }

   static void InitServer(const ServerType& server_type, const EpollTriggerMode& trigger_mode, const std::string& listen_addr, const int& listen_port, const int& parallel, const int& msg_processor_num) {
     if (instance_ == nullptr) {
       std::call_once(instance_guard_, [=]() {
         if (server_type == ServerType::TCP) {
           //EpollRunMode mode = EpollRunMode::UseProcess;
           EpollRunMode mode = EpollRunMode::UseThread;
           instance_ = new EpollTCPServer(mode, trigger_mode, parallel, msg_processor_num, listen_addr, listen_port);
         } else if (server_type == ServerType::UDP) {
           instance_ = new EpollUDPServer(listen_addr, listen_port);
         }
       });
     }
     std::signal(45, EpollServerFactory::SignalHandler);
   }
 private:
  EpollServerFactory(const EpollServerFactory&) = delete;
  static std::once_flag instance_guard_;
  static EpollServerBase* instance_;
  static void SignalHandler(int signal) {
    instance_->Stop();
  }
};

/* static */
std::once_flag EpollServerFactory::instance_guard_;
EpollServerBase* EpollServerFactory::instance_ = nullptr;

#endif
