#pragma once
#include <functional>
#include "Macros.h"

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection {
 public:
  enum State { Invalid = 1, Handshaking, Connected, Closed, Failed };
  Connection(EventLoop *loop, Socket *sock);
  ~Connection();

  DISALLOW_COPY_AND_MOVE(Connection)

  void Read();
  void Write();
  void Close();

  State GetState();
  Buffer *GetReadBuffer();
  const char *ReadBuffer();
  Buffer *GetSendBuffer();
  const char *SendBuffer();
  void SetSendBuffer(const char *str);
  void GetlineSendBuffer();
  Socket *GetSocket();

  void SetDeleteConnectionCallBack(std::function<void(Socket *)> const &callback);
  void SetOnConnectCallBack(std::function<void(Connection *)> const &callback);

 private:
  void ReadNonBlocking();
  void WriteNonBlocking();
  void ReadBlocking();
  void WriteBlocking();

  EventLoop *loop_;
  Socket *sock_;
  Channel *channel_{nullptr};
  State state_{State::Invalid};
  Buffer *read_buffer_{nullptr};
  Buffer *send_buffer_{nullptr};

  std::function<void(Socket *)> delete_connection_callback_;
  std::function<void(Connection *)> on_connect_callback_;
};
