#pragma once
#include <functional>
class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
 private:
  Epoll* ep;
  ThreadPool* thread_pool;
  bool quit;

 public:
  EventLoop();
  ~EventLoop();

  void loop();
  void update_channel(Channel*);
  void add_thread(std::function<void()>);
};