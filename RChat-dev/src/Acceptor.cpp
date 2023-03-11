#include "Acceptor.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

// 构造 Acceptor 实例
Acceptor::Acceptor(EventLoop* _loop): loop(_loop) {
    socket = new Socket();
    addr = new InetAddress(PORT_NUM);
    socket->set_reuseaddr();
    socket->bind(addr);
    socket->listen();
    accpet_channel = new Channel(loop, socket->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accpet_channel->set_callback(cb);
    accpet_channel->enable_reading();
    delete addr;
}

Acceptor::~Acceptor() {
    delete socket;
    delete accpet_channel;
}

// 实际调用的 Server::new_connection 函数进行连接操作
void Acceptor::accept_connection() {
    InetAddress *client_addr = new InetAddress(PORT_NUM);   
    Socket *client_socket = new Socket(socket->accept(client_addr));     // 内存泄露
    printf("new client connected! fd: %d, IP: %s, Port: %d\n", 
        client_socket->get_fd(), inet_ntoa(client_addr->addr.sin_addr), ntohs(client_addr->addr.sin_port));
    new_connection_callback(client_socket);
    delete client_addr;
}

void Acceptor::set_new_connection_callback(std::function<void(Socket*)> _cb) {
    new_connection_callback = _cb;
}