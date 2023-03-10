#include "Channel.h"
#include "EventLoop.h"
#include <cstdio>

Channel::Channel(EventLoop* _loop, int _fd) : loop(_loop), fd(_fd), events(0), revents(0), in_epoll(false) {
}

Channel::~Channel() {
    delete loop;
}

// 监控可读，并且添加到内核事件表。
void Channel::enable_reading() {
    events = EPOLLIN;
    loop->update_channel(this);
}

int Channel::get_fd() {
    return fd;
}

uint32_t Channel::get_events() {
    
    return events;
}

uint32_t Channel::get_revents() {
    return revents;
}

bool Channel::get_in_epoll() {
    return in_epoll;
}

void Channel::set_in_epoll() {
    in_epoll = true;
}

void Channel::set_revents(uint32_t _rev) {
    revents = _rev;
}

void Channel::handle_event() {
    callback();
}

void Channel::set_callback(std::function<void()> _cb) {
    callback = _cb;
}