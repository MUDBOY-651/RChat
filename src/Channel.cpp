#include "include/Channel.h"

#include <unistd.h>

#include <cstdio>

#include "include/EventLoop.h"

Channel::Channel(EventLoop* _loop, int _fd)
    : loop(_loop), fd(_fd), events(0), revents(0), in_epoll(false) {}

Channel::~Channel() {
  if (fd != -1) {
    close(fd);
    fd = -1;
  }
}

// 监控可读，并且添加到内核事件表。
void Channel::enable_reading() {
  events |= EPOLLIN | EPOLLPRI;
  loop->update_channel(this);
}

int Channel::get_fd() { return fd; }

uint32_t Channel::get_events() { return events; }

uint32_t Channel::get_revents() { return revents; }

bool Channel::get_in_epoll() { return in_epoll; }

void Channel::set_in_epoll(bool in) { in_epoll = in; }

void Channel::set_revents(uint32_t _rev) { revents = _rev; }

// 回调read(EPOLLIN, EPOLLPRI), 回调write(EPOLLOUT)
void Channel::handle_event() {
  // callback();
  if (revents & (EPOLLIN | EPOLLPRI)) {
    read_callback();
  }
  if (revents & (EPOLLOUT)) {
    write_callback();
  }
}

void Channel::use_ET() {
  events |= EPOLLET;
  loop->update_channel(this);
}

void Channel::set_read_callback(std::function<void()> _cb) {
  read_callback = _cb;
}

