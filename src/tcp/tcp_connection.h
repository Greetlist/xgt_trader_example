#ifndef __TCP_CONNECTION_H_
#define __TCP_CONNECTION_H_

#include <arpa/inet.h>
#include <sys/uio.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <map>
#include <mutex>
#include <ctime>

#include <functional>

#include "logger/logger.h"
#include "buffer/vec_buffer.h"
#include "interface/message_coder.h"
#include "epoll_server/epoll_tcp_server.h"
#include "queue/queue_msg.h"

class MessageInfo;
class TcpConnection {
public:
  explicit TcpConnection(int);
  ~TcpConnection();
  void Init();
  int Connect();
  void Close();
  int Read();
  void TryFlushWriteBuffer();
  std::vector<MessageInfo*> ExtractMessage();
  int Write(char*, int);
  int GetSocketFd();
  void SetLastActiveTime(int);
  bool IsExpire();
private:
  VecBuffer read_buffer_;
  VecBuffer write_buffer_;
  int socket_fd_;
  int latest_message_type_;
  int latest_message_len_;
  int total_handle_msg_;
  std::atomic<int> last_active_time_;
  std::mutex read_lock_;
  std::mutex write_lock_;
  static constexpr int INT_SIZE = sizeof(int);
  static constexpr int ActiveTimeout = 60;
};
#endif
