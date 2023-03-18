#pragma once
#include <functional>
#include "Macros.h"
class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor {
 private:
  EventLoop* loop;
  Socket* socket;  // 监听 socket
  Channel* accpet_channel;
  std::function<void(Socket*)> new_connection_callback;

 public:
  Acceptor(EventLoop* loop);
  ~Acceptor();
  DISALLOW_COPY_AND_MOVE(Acceptor);
  void accept_connection();
  void set_new_connection_callback(
      std::function<void(Socket*)>);  // 设置建立连接的 function
};
