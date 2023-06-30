#include "interface/xgt_client.h"

namespace XGT {

XGTClient::XGTClient(const char* server, const int& port) : server_addr_(server), port_(port) {
}

XGTClient::~XGTClient() {
  LOG_INFO("Total Write bytes: %ld", total_write_bytes_);
}

int XGTClient::Read() {
  // read packet size and packet type
  int packet_info[2];
  int n_read = read(client_fd_, static_cast<void*>(packet_info), sizeof(int) * 2);

  int total_packet_size = packet_info[0];
  int block_num = total_packet_size / buf_size_ + 1;
  struct iovec read_vec[block_num];
  char* read_buf = new char[total_packet_size];
  memset(read_buf, 0, total_packet_size);
  for (int i = 0; i < block_num; ++i) {
    read_vec[i].iov_base = read_buf + i * buf_size_ ;
    read_vec[i].iov_len = buf_size_;
  }
  n_read = readv(client_fd_, read_vec, block_num);

  int packet_type = packet_info[1];
  DispatchPacket(packet_type, read_buf, total_packet_size);
  return n_read;
}

void XGTClient::DispatchPacket(int packet_type, char* data, int total_packet_size) {
  switch (packet_type) {
  case LoginResponse:
    break;
  case LogoutResponse:
    break;
  case SubscribeResponse:
    break;
  case ReturnOrder:
    break;
  case ReturnTrade:
    break;
  case QueryAccountResponse:
    break;
  case QueryPositionResponse:
    break;
  case QueryOrderResponse:
    break;
  case QueryTradeResponse:
    break;
  default:
    break;
  }
  free(data);
}

void XGTClient::Connect() {
  client_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd_ < 0) {
      LOG_ERROR("Create Socket Error: %s", strerror(errno));
      std::exit(1);
  }
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, server_addr_.c_str(), &addr.sin_addr);
  addr.sin_port = htons(port_);
  int connect_status = connect(client_fd_, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
  if (connect_status < 0) {
      LOG_ERROR("Connect [ %s:%d ] Error: %s", server_addr_.c_str(), port_, strerror(errno));
      std::exit(1);
  }
  SetNonBlocking(client_fd_);
}

int XGTClient::SetNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  int status = fcntl(fd, F_SETFL, flags);
  if (status < 0) {
    LOG_ERROR("File Control Error: %s", strerror(errno));
    std::exit(1);
  }
  return status;
}

int XGTClient::GetSocket() {
  return client_fd_;
}

void XGTClient::SetSpi(XGTTraderSpi* spi) {
  spi_ = spi;
}

} //namespace XGT
