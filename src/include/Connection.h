#pragma once
#include <functional>
#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection {
 public:
  Connection(EventLoop *loop, Socket *sock);
  ~Connection();

  DISALLOW_COPY_AND_MOVE(Connection)

  void Echo(int sockfd);
  void Send(int sockfd);
  void SetDeleteConnectionCallBack(std::function<void(int)> const &callback);

 private:
  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_;
  std::function<void(int)> delete_connection_callback_;
  Buffer *read_buffer_;
};
