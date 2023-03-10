#pragma once
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel {
private :
    EventLoop* loop;
    int fd;
    uint32_t events;    // 希望监听哪些事件
    uint32_t revents;   // 在epoll返回该Channel时文件描述符正在发生的事件
    bool in_epoll;  // 当前 Channel 是否在 epoll 红黑树中。（使用 EPOLL_CTL_ADD or EPOLL_CTL_MOD)
    std::function<void()> callback;
public:
    Channel(EventLoop* _loop, int _fd);
    ~Channel();
    void enable_reading();
    int get_fd();
    uint32_t get_events();
    uint32_t get_revents();
    bool get_in_epoll(); 
    void set_in_epoll();
    
    void set_revents(uint32_t);
    void handle_event();
    void set_callback(std::function<void()>);
};