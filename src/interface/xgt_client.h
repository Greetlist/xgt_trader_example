#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/uio.h>

#include "interface/logger.h"
#include "interface/xgt_trader_spi.h"

namespace XGT {

class XGTClient {
public:
  explicit XGTClient(const std::string& server, const int& port);
  XGTClient& operator=(const XGTClient&) = delete;
  XGTClient(const XGTClient&) = delete;
  ~XGTClient();
  int Write(char* data, int data_len);
  int Read();
  void Connect();
  int DisConnect();
  int GetSocket();
  void SetSpi(XGTTraderSpi* spi);
private:
  int SetNonBlocking(int fd);
  void DispatchPacket(int packet_type, char* data, int total_packet_size);
  void FreeData(char* data);
  std::string server_addr_;
  static constexpr int buf_size_ = 65536;
  char* input_buf_;
  char* output_buf_;
  XGTTraderSpi* spi_;
  int port_;
  int client_fd_;
};

}// namespace XGT

#endif
