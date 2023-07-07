#include "tcp/tcp_connection.h"

TcpConnection::TcpConnection(int sock_fd) : socket_fd_(sock_fd), latest_message_type_(-1), latest_message_len_(-1), total_handle_msg_(0) {
}

TcpConnection::~TcpConnection() {
  LOG_INFO(
    "Total Read Bytes: %ld, Total Write Bytes: %ld",
    read_buffer_.GetTotalHandleBytes(),
    write_buffer_.GetTotalHandleBytes()
  );
  LOG_INFO(
    "ReadBuffer enlarge count: %d, WriteBuffer enlarge count: %d",
    read_buffer_.GetEnlargeCount(),
    write_buffer_.GetEnlargeCount()
  );
  LOG_INFO("Total Handle Message: %d", total_handle_msg_);
}

void TcpConnection::Init() {
  read_buffer_.Init();
  write_buffer_.Init();
}

int TcpConnection::Read() {
  return read_buffer_.ReadFromFd(socket_fd_);
}

std::vector<MessageInfo*> TcpConnection::ExtractMessage() {
  std::vector<MessageInfo*> res;
  int total_handle_bytes = 0;
  int unhandle_bytes = read_buffer_.GetUnHandleBytesNum();
  char* read_index = read_buffer_.GetReadIndex();
  while (true) {
    if (unhandle_bytes <= 0) {
      break;
    }

    //read message type
    if (latest_message_type_ == -1 && unhandle_bytes < INT_SIZE) {
      break;
    }
    if (latest_message_type_ == -1 && unhandle_bytes >= INT_SIZE) {
      uint32_t cur;
      memcpy(&cur, read_index, INT_SIZE);
      latest_message_type_ = ntohl(cur);
      read_index += INT_SIZE;
      total_handle_bytes += INT_SIZE;
      unhandle_bytes -= INT_SIZE;
    }

    //read message len
    if (latest_message_len_ == -1 && unhandle_bytes < INT_SIZE) {
      break;
    }
    if (latest_message_len_ == -1 && unhandle_bytes >= INT_SIZE) {
      uint32_t cur;
      memcpy(&cur, read_index, INT_SIZE);
      latest_message_len_ = ntohl(cur);
      read_index += INT_SIZE;
      total_handle_bytes += INT_SIZE;
      unhandle_bytes -= INT_SIZE;
    }

    // current data is not enough to construct one message
    if (latest_message_len_ > unhandle_bytes) {
      break;
    }

    //has enough data for construction
    char message[latest_message_len_];
    memcpy(message, read_index, latest_message_len_);
    MessageInfo* p = new MessageInfo(this, latest_message_type_, std::move(std::string(message, latest_message_len_)));
    res.push_back(p);
    total_handle_msg_++;

    //update buffer info
    read_index += latest_message_len_;
    total_handle_bytes += latest_message_len_;
    unhandle_bytes -= latest_message_len_;
    latest_message_type_ = -1;
    latest_message_len_ = -1;
  }
  read_buffer_.IncrReadIndex(total_handle_bytes);
  return res;
}

int TcpConnection::Write(char* data, int data_len) {
  write_buffer_.SaveData(data, data_len);
  write_buffer_.WriteToFd(socket_fd_);
  return 0;
}

int TcpConnection::GetSocketFd() {
  return socket_fd_;
}
