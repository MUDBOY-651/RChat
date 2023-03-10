#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h" 
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    printf("Waiting for connection...");
    while (!quit) {
        std::vector<Channel*> chs;
        chs = ep->poll();
        for (auto it: chs) {
            it->handle_event();
        }
    }
}

void EventLoop::update_channel(Channel *ch) {
    ep->update_channel(ch);
}