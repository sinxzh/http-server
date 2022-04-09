#ifndef HTTP_SERVER_HTTP_CONN_HTTP_CONN_HPP
#define HTTP_SERVER_HTTP_CONN_HTTP_CONN_HPP

#include <cstdlib>
#include <iostream>

static constexpr size_t REQ_MSG_BUF_SIZE = 1024;
static constexpr size_t RESP_MSG_BUF_SIZE = 1024;

class HttpConn {
 public:
  HttpConn() = default;
  ~HttpConn() = default;

  void process();

 private:
  char reqMsgBuf_[REQ_MSG_BUF_SIZE];
  char respMsgBuf_[RESP_MSG_BUF_SIZE];
};

void HttpConn::process() {
  std::cout << "解析请求报文，生成响应报文"
            << ",   ";
}

#endif
