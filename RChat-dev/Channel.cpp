#include "Channel.h"
#include "Epoll.h"
#include <cstdio>

Channel::Channel(Epoll* _ep, int _fd) : ep(_ep), fd(_fd), events(0), revents(0), in_epoll(false) {
}

Channel::~Channel() {
    ep = nullptr;
}

// 监控可读，并且添加到内核事件表。
void Channel::enable_reading() {
    events = EPOLLIN;
    ep->updateChannel(this);
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
    puts("start setting");
    revents = _rev;
    puts("set revents success");
}