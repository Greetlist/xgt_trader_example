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
#include <nlohmann/json.hpp>

#include "logger/logger.h"
#include "interface/util.h"
#include "interface/xgt_trader_spi.h"
#include "interface/message_coder.h"

namespace XGT {

static constexpr uint32_t INT_SIZE = sizeof(int);

class XGTClient {
public:
  explicit XGTClient(const char* server, const int& port);
  XGTClient& operator=(const XGTClient&) = delete;
  XGTClient(const XGTClient&) = delete;
  ~XGTClient();
  template <typename T>
  int Write(const T& req) {
    nlohmann::json req_json = MessageCoder::StructToJson(req);
    std::string json_str = req_json.dump();
    uint32_t struct_size = json_str.size();
    char buf[INT_SIZE * 2 + struct_size];
    int request_type = GetRequestType<T>(req);
    //std::cout << json_str << " " << json_str.size() << " " << request_type << std::endl;
    uint32_t request_size = struct_size;
    uint32_t req_size_no = htonl(request_size);
    uint32_t req_type_no = htonl(request_type);
    memcpy(buf, &req_type_no, INT_SIZE);
    memcpy(buf+INT_SIZE, &req_size_no, INT_SIZE);
    memcpy(buf+INT_SIZE*2, json_str.c_str(), struct_size);

    int total_bytes = INT_SIZE * 2 + struct_size;
    int write_index = 0;
    while (total_bytes > 0) {
      int res = write(client_fd_, buf + write_index, total_bytes);
      if (res < 0) {
        if (errno != EAGAIN) {
          LOG_ERROR("Write Error: %s", strerror(errno));
        }
      } else if (res == 0) {
        LOG_WARN("Connection is closed, Start Reconnect");
        Connect();
      } else {
        total_write_bytes_ += res;
        total_bytes -= res;
        write_index += res;
      }
    }
    return 0;
  }

  int Read();
  void Connect();
  int DisConnect();
  int GetSocket();
  void SetSpi(XGTTraderSpi* spi);
private:
  int SetNonBlocking(int fd);
  void DispatchPacket(int packet_type, char* data, int total_packet_size);
  void FreeData(char* data);
  uint64_t total_write_bytes_;
  std::string server_addr_;
  char* input_buf_;
  char* output_buf_;
  XGTTraderSpi* spi_;
  int port_;
  int client_fd_;
  static constexpr int buf_size_ = 65536;
};

}// namespace XGT

#endif
