#include "Acceptor.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

// 构造 Acceptor 实例
Acceptor::Acceptor(EventLoop* _loop): loop(_loop) {
    socket = new Socket();
    addr = new InetAddress(3389);
    socket->bind(addr);
    socket->listen();
    accpet_channel = new Channel(loop, socket->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accpet_channel->set_callback(cb);
    accpet_channel->enable_reading();
}

Acceptor::~Acceptor() {
    delete socket;
    delete addr;
    delete accpet_channel;
}

// 实际调用的 Server::new_connection 函数进行连接操作
void Acceptor::accept_connection() {
    new_connection_callback(socket);
}

void Acceptor::set_new_connection_callback(std::function<void(Socket*)> _cb) {
    new_connection_callback = _cb;
}