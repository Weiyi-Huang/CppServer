#include "Socket.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include "Util.h"

InetAddress::InetAddress() = default;

InetAddress::InetAddress(const char *ip, uint16_t port) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = inet_addr(ip);
  addr_.sin_port = htons(port);
}

void InetAddress::SetAddr(sockaddr_in addr) { addr_ = addr; }

sockaddr_in InetAddress::GetAddr() { return addr_; }

const char *InetAddress::GetIp() { return inet_ntoa(addr_.sin_addr); }

uint16_t InetAddress::GetPort() { return ntohs(addr_.sin_port); }

Socket::Socket() {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  ErrorIf(fd_ == -1, "Socket create error");
}

Socket::Socket(int fd) : fd_(fd) { ErrorIf(fd_ == -1, "Socket create error"); }

Socket::~Socket() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Socket::Bind(InetAddress *addr) {
  sockaddr_in temp_addr = addr->GetAddr();
  ErrorIf(::bind(fd_, (sockaddr *)&temp_addr, sizeof(temp_addr)) == -1, "Socket bind error");
}

void Socket::Listen() { ErrorIf(::listen(fd_, SOMAXCONN) == -1, "Socket listen error"); }

int Socket::Accept(InetAddress *addr) {
  // for server socket
  int clnt_sockfd = -1;
  sockaddr_in temp_addr{};
  socklen_t addr_len = sizeof(temp_addr);
  if (IsNonBlocking()) {
    while (true) {
      clnt_sockfd = ::accept(fd_, (sockaddr *)&temp_addr, &addr_len);
      if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        printf("No connection yet\n");
        continue;
      }
      if (clnt_sockfd == -1) {
        ErrorIf(true, "Socket accept error!");
      } else {
        break;
      }
    }
  } else {
    clnt_sockfd = ::accept(fd_, (sockaddr *)&temp_addr, &addr_len);
    ErrorIf(clnt_sockfd == -1, "Socket accept error!");
  }
  addr->SetAddr(temp_addr);
  return clnt_sockfd;
}

void Socket::Connect(InetAddress *addr) {
  // for client socket
  sockaddr_in temp_addr = addr->GetAddr();
  if (IsNonBlocking()) {
    while (true) {
      int ret = ::connect(fd_, (sockaddr *)&temp_addr, sizeof(temp_addr));
      if (ret == 0) {
        break;
      }
      if (ret == -1 && (errno == EINPROGRESS)) {
        continue;
      }
      if (ret == -1) {
        ErrorIf(true, "Socket connect error");
      }
    }
  } else {
    ErrorIf(::connect(fd_, (sockaddr *)&temp_addr, sizeof(temp_addr)) == -1, "Socket connect error");
  }
}

void Socket::Connect(const char *ip, uint16_t port) {
  InetAddress *addr = new InetAddress(ip, port);
  Connect(addr);
  delete addr;
}

void Socket::SetNonBlocking() { fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK); }

bool Socket::IsNonBlocking() { return (fcntl(fd_, F_GETFL) & O_NONBLOCK) != 0; }

int Socket::GetFd() { return fd_; }
