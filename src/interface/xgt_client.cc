#include "interface/xgt_client.h"

XGTClient::XGTClient(const std::string& server, const int& port) : server_addr_(server), port_(port) {
  input_buf_ = new char[buf_size_];
  output_buf_ = new char[buf_size_];
}

XGTClient::~XGTClient() {
  delete input_buf_;
  delete output_buf_;
}

int XGTClient::Write(char* data, int data_len) {
  return 0;
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
  return n_read;
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
  addr.sin_port = htons(12345);
  int connect_status = connect(client_fd_, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
  if (connect_status < 0) {
      LOG_ERROR("Connect Error: %s", strerror(errno));
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

