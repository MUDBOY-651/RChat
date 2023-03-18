#include "include/InetAddress.h"

#include <cstring>

InetAddress::InetAddress() : addr_len(sizeof addr) {
  bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(const char* ip, uint16_t port) {
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(port);
  addr_len = sizeof(addr);
}

InetAddress::InetAddress(uint16_t port) {
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  addr_len = sizeof(addr);
}

InetAddress::~InetAddress() {}