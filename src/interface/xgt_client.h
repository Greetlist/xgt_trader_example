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
#include <type_traits>

#include "interface/logger.h"
#include "interface/xgt_trader_spi.h"

namespace XGT {

static constexpr int INT_SIZE = sizeof(int);

class XGTClient {
public:
  explicit XGTClient(const char* server, const int& port);
  XGTClient& operator=(const XGTClient&) = delete;
  XGTClient(const XGTClient&) = delete;
  ~XGTClient();
  template <typename T>
  int Write(const T& req) {
    int struct_size = sizeof(T);
    char buf[INT_SIZE * 2 + struct_size];
    int request_type = GetRequestType<T>(req);
    int request_size = INT_SIZE * 2 + struct_size;
    memcpy(buf, &request_type, INT_SIZE);
    memcpy(buf+INT_SIZE, &request_size, INT_SIZE);
    memcpy(buf+INT_SIZE*2, &req, struct_size);
    int res = write(client_fd_, buf, INT_SIZE * 2 + struct_size);
    if (res < 0) {
      LOG_ERROR("Write Error: %s", strerror(errno));
      return res;
    } else if (res == 0) {
      LOG_WARN("Connection is closed, Start Reconnect");
      Connect();
    }
    return 0;
  }

  int Read();
  void Connect();
  int DisConnect();
  int GetSocket();
  void SetSpi(XGTTraderSpi* spi);
private:
  template <typename T>
  int GetRequestType(const T& req) {
    if (std::is_same<T, XGTLoginRequest>::value) {
      return RequestType::LoginRequest;
    } else if (std::is_same<T, XGTLogoutRequest>::value) {
      return RequestType::LogoutRequest;
    } else if (std::is_same<T, XGTSubscribeRequest>::value) {
      return RequestType::SubscribeRequest;
    } else if (std::is_same<T, XGTInsertOrderRequest>::value) {
      return RequestType::InsertOrderRequest;
    } else if (std::is_same<T, XGTCancelOrderRequest>::value) {
      return RequestType::CancelOrderRequest;
    } else if (std::is_same<T, XGTQueryCommonRequest>::value) {
      return RequestType::QueryRequest;
    }
    return RequestType::UnknownRequest;
  }
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
