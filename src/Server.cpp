#include "Server.h"
#include <unistd.h>
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "Util.h"

Server::Server(EventLoop *loop) : main_reactor_(loop), acceptor_(nullptr), thread_pool_(nullptr) {
  acceptor_ = new Acceptor(main_reactor_);
  std::function<void(Socket *)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
  acceptor_->SetNewConnectionCallback(cb);

  int size = static_cast<int>(std::thread::hardware_concurrency());
  thread_pool_ = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    sub_reactors_.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i]);
    thread_pool_->Add(std::move(sub_loop));
  }
}

Server::~Server() {
  delete acceptor_;
  delete thread_pool_;
}

void Server::NewConnection(Socket *sock) {
  ErrorIf(sock->GetFd() == -1, "New connection error");
  uint64_t random = sock->GetFd() % sub_reactors_.size();
  Connection *conn = new Connection(sub_reactors_[random], sock);
  std::function<void(Socket *)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  conn->SetDeleteConnectionCallBack(cb);
  conn->SetOnConnectCallBack(on_connect_callback_);
  connections_[sock->GetFd()] = conn;
}

void Server::DeleteConnection(Socket *sock) {
  int sockfd = sock->GetFd();
  auto it = connections_.find(sockfd);
  if (it != connections_.end()) {
    Connection *conn = connections_[sockfd];
    connections_.erase(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void Server::OnConnect(std::function<void(Connection *)> fn) { on_connect_callback_ = std::move(fn); }
