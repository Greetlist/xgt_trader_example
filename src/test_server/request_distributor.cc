#include "server/request_distributor.h"

namespace XGT {

void DistributeRequest(int n_read, char* buf) {
  std::cout << "Read: " << n_read << "bytes data, data is: " << buf;
  //char buf[RequestMaxBufferSize];
  //memset(buf, 0, RequestMaxBufferSize);

  //int request_type, request_size;
  //int n_read = read(fd, buf, INT_SIZE);
  //memcpy(&request_size, buf, INT_SIZE);
  //n_read += read(fd, buf+INT_SIZE, request_size);
  //memcpy(&request_type, buf + INT_SIZE, INT_SIZE);
  //GetRequestType<request_type>() request;
  //memcpy(&request, buf + 2 * INT_SIZE; request_size - 2 * INT_SIZE);
  //DispatchRequest(request_type, request_size - INT_SIZE, buf + 2 * INT_SIZE);
  //std::cout << "RequestType: " << request_type << ", RequestSize: " << request_size << std::endl;
}

void DispatchRequest(int request_type, int data_len, char* data) {
  if (request_type == 1) {
    XGTLoginRequest req;
    memcpy(&req, data, data_len);
    std::cout << "Login: " << req.account << " " << req.password << std::endl;
  } else if (request_type == 2) {
    XGTLogoutRequest req;
    memcpy(&req, data, data_len);
    std::cout << "Logout: " << req.account << std::endl;
  } else if (request_type == 3) {
    XGTSubscribeRequest req;
    memcpy(&req, data, data_len);
    std::cout << "Subscribe: " << req.channel << std::endl;
  } else if (request_type == 4) {
    XGTInsertOrderRequest req;
    memcpy(&req, data, data_len);
    std::cout << "Order: " << req.instrument_id << " " << req.exchange_id << " " << req.fund_account << std::endl;
  } else if (request_type == 5) {
    XGTCancelOrderRequest req;
    memcpy(&req, data, data_len);
    std::cout << "Cancel:" << req.order_sys_id << " " << req.exchange_id << std::endl;
  }
}

} //namespace XGT
