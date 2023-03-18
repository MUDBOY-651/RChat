#pragma once
#include <functional>
#include "Macros.h"
class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
 private:
  Epoll* ep;
  bool quit;

 public:
  EventLoop();
  ~EventLoop();
  DISALLOW_COPY_AND_MOVE(EventLoop);

  void loop();
  void update_channel(Channel*);
};