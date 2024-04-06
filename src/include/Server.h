#pragma once
#include <functional>
#include <map>
#include <vector>
#include "Macros.h"

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
 public:
  explicit Server(EventLoop *loop);
  ~Server();

  DISALLOW_COPY_AND_MOVE(Server)

  void NewConnection(Socket *sock);
  void DeleteConnection(Socket *sock);
  void OnConnect(std::function<void(Connection *)> fn);

 private:
  EventLoop *main_reactor_;
  Acceptor *acceptor_;
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_reactors_;
  ThreadPool *thread_pool_;
  std::function<void(Connection *)> on_connect_callback_;
};
