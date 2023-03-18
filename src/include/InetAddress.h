#pragma once
#include <arpa/inet.h>
#define PORT_NUM 3389

class InetAddress {
 public:
  struct sockaddr_in addr;
  socklen_t addr_len;
  InetAddress();
  InetAddress(const char* ip, uint16_t port);
  // address to accpet any incoming messages.
  InetAddress(uint16_t port);
  ~InetAddress();
};