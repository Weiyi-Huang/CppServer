#pragma once
#include <arpa/inet.h>
#include "Macros.h"

class InetAddress {
 public:
  InetAddress();
  InetAddress(const char *ip, uint16_t port);
  ~InetAddress() = default;

  DISALLOW_COPY_AND_MOVE(InetAddress)

  void SetAddr(sockaddr_in addr);
  sockaddr_in GetAddr();
  const char *GetIp();
  uint16_t GetPort();

 private:
  sockaddr_in addr_{};
};

class Socket {
 public:
  Socket();
  explicit Socket(int fd);
  ~Socket();

  DISALLOW_COPY_AND_MOVE(Socket)

  void Bind(InetAddress *addr);
  void Listen();
  int Accept(InetAddress *addr);

  void Connect(InetAddress *addr);
  void Connect(const char *ip, uint16_t port);

  void SetNonBlocking();
  bool IsNonBlocking();
  int GetFd();

 private:
  int fd_{-1};
};
