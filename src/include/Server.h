#pragma once
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
  void DeleteConnection(int sockfd);

 private:
  EventLoop *main_reactor_;
  Acceptor *acceptor_;
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_reactors_;
  ThreadPool *thread_pool_;
};
