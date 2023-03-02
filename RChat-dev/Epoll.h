#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll {
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();
    void add_fd(int fd, uint32_t op);
    void del_fd(int fd);
    std::vector<epoll_event> poll(int timeout = -1);
    int get_fd();
};