#pragma once
#include "Macros.h"
class InetAddress;
class Socket {
 private:
  int fd;

 public:
  Socket();
  Socket(int);
  ~Socket();
  DISALLOW_COPY_AND_MOVE(Socket);
  void bind(InetAddress*);
  void listen();
  void set_nonblocking();  // 设置非阻塞 socket
  void set_reuseaddr();
  int accept(InetAddress*);
  void connect(InetAddress*);
  int get_fd();
};
