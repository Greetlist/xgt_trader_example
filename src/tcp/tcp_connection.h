#ifndef __TCP_CONNECTION_H_
#define __TCP_CONNECTION_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <string.h>
#include <stdint.h>

#include <functional>

#include "logger/logger.h"
#include "buffer/vec_buffer.h"
#include "interface/message_coder.h"

class TcpConnection {
public:
  explicit TcpConnection(int);
  ~TcpConnection();
  void Init();
  int Read();
  int ExtractMessage();
  void QueueMessage(const std::string&&);
  int Write(char*, int);
  int GetSocketFd();
private:
  VecBuffer read_buffer_;
  VecBuffer write_buffer_;
  int socket_fd_;
  int latest_message_type_;
  int latest_message_len_;
  static constexpr int INT_SIZE = sizeof(int);
};

#endif
