#include "tcp/tcp_connection.h"

TcpConnection::TcpConnection(int sock_fd) : socket_fd_(sock_fd), latest_message_type_(-1), latest_message_len_(-1) {
}

TcpConnection::~TcpConnection() {
  LOG_INFO("Total Read Bytes: %ld", read_buffer_.GetTotalHandleBytes());
  LOG_INFO("Total Write Bytes: %ld", write_buffer_.GetTotalHandleBytes());
}

void TcpConnection::Init() {
  read_buffer_.Init();
  write_buffer_.Init();
}

int TcpConnection::Read() {
  return read_buffer_.ReadFromFd(socket_fd_);
}

int TcpConnection::ExtractMessage() {
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
    if (latest_message_len_ > unhandle_bytes - INT_SIZE) {
      break;
    }

    //has enough data for construction
    char message[latest_message_len_];
    memcpy(message, read_index, latest_message_len_);
    QueueMessage(message, latest_message_len_);
    total_handle_bytes += latest_message_len_;
    unhandle_bytes -= latest_message_len_;
    latest_message_type_ = -1;
  }
  read_buffer_.IncrReadIndex(total_handle_bytes);
  return total_handle_bytes;
}

void TcpConnection::QueueMessage(char* message, int message_len) {
  LOG_INFO("message_type is: %d, message_len: %d", latest_message_type_, message_len);
  std::string json_stream{message};
  nlohmann::json j = nlohmann::json::parse(json_stream);
  LOG_INFO("json is: %s", j.dump().c_str());
  XGT::XGTRequest req = MessageCoder::JsonToRequest(latest_message_type_, j);
}

int TcpConnection::Write(char* data, int data_len) {
  write_buffer_.SaveData(data, data_len);
  return 0;
}

int TcpConnection::GetSocketFd() {
  return socket_fd_;
}
