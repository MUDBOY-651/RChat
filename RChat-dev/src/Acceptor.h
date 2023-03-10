#pragma once
#include <functional>
class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor {
private:
    EventLoop *loop;
    Socket* socket;     // 监听 socket
    InetAddress* addr;  
    Channel* accpet_channel;
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();
    void accept_connection();
    std::function<void(Socket*)> new_connection_callback;
    void set_new_connection_callback(std::function<void(Socket*)>); // 设置建立连接的 function
};
