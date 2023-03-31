#include "epoll_server/server_factory.h"
#include "epoll_server/global_def.h"

#include <unordered_map>
#include <string>
#include <iostream>

void test_func(char* data, int data_len) {
  std::cout << data << std::endl;
}

int main(int argc, char** argv) {
  std::unordered_map<std::string, std::string> config {
    {"listen_addr", "0.0.0.0"},
    {"listen_port", "10000"},
  };
  ftcp::EpollServerBase* server = ftcp::EpollServerFactory::GetServer(ftcp::ServerType::TCP, config["listen_addr"], std::stoi(config["listen_port"]));
  server->Init();
  server->SetDataCallback(test_func);
  server->Start();
  return 0;
}
