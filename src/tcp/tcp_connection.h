#ifndef __TCP_CONNECTION_H_
#define __TCP_CONNECTION_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <string.h>
#include <stdint.h>
#include <map>

#include <functional>

#include "logger/logger.h"
#include "buffer/vec_buffer.h"
#include "interface/message_coder.h"
#include "epoll_server/epoll_tcp_server.h"

class EpollTCPServer;
class TcpConnection {
public:
  explicit TcpConnection(int, EpollTCPServer*);
  ~TcpConnection();
  void Init();
  int Read();
  int ExtractMessage();
  void QueueMessage(std::pair<int, std::string*>*);
  int Write(char*, int);
  int GetSocketFd();
  EpollTCPServer* server_;
private:
  VecBuffer read_buffer_;
  VecBuffer write_buffer_;
  int socket_fd_;
  int latest_message_type_;
  int latest_message_len_;
  int total_handle_msg_;
  static constexpr int INT_SIZE = sizeof(int);
};

#endif
