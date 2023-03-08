#pragma once
#include <sys/epoll.h>

class Epoll;
class Channel {
private :
    Epoll *ep;  // 指向被分配到的 Epoll 对象
    int fd;
    uint32_t events;    // 希望监听哪些事件
    uint32_t revents;   // 在epoll返回该Channel时文件描述符正在发生的事件
    bool in_epoll;  // 当前 Channel 是否在 epoll 红黑树中。（使用 EPOLL_CTL_ADD or EPOLL_CTL_MOD)
public:
    Channel(Epoll* _ep, int _fd);
    ~Channel();
    void enable_reading();
    int get_fd();
    uint32_t get_events();
    uint32_t get_revents();
    bool get_in_epoll(); 
    void set_in_epoll();

    void set_revents(uint32_t);
};