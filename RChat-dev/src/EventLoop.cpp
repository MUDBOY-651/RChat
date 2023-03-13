#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h" 
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), thread_pool(nullptr), quit(false) {
    ep = new Epoll();
    thread_pool = new ThreadPool();
}

EventLoop::~EventLoop() {
    delete thread_pool;
    delete ep;
}

void EventLoop::loop() {
    printf("Waiting for connection...\n");
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

void EventLoop::add_thread(std::function<void()> func) {
    thread_pool->add(func);
}