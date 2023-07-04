#ifndef __QUEUE_MSG_H_
#define __QUEUE_MSG_H_

#include "tcp/tcp_connection.h"

class TcpConnection;
class MessageInfo {
public:
  MessageInfo(TcpConnection* c, const int mt, const std::string&& mj) : conn(c), message_type(mt), message_json(mj) {}
  ~MessageInfo() = default;
  TcpConnection* conn;
  int message_type;
  std::string message_json;
};


#endif
