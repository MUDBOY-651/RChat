#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();
    // std::vector<epoll_event> poll(int timeout = -1);
    std::vector<Channel*> poll(int timeout = -1);
    void update_channel(Channel* ch);
    int get_fd();
};