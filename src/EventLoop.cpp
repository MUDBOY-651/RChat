#include "include/EventLoop.h"

#include <vector>

#include "include/Channel.h"
#include "include/Epoll.h"
#include "include/ThreadPool.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) {
  ep = new Epoll();
}

EventLoop::~EventLoop() { delete ep; }

void EventLoop::loop() {
  while (!quit) {
    std::vector<Channel *> chs;
    chs = ep->poll();
    for (auto it : chs) {
      it->handle_event();
    }
  }
}

void EventLoop::update_channel(Channel *ch) { ep->update_channel(ch); }