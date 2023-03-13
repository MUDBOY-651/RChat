#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Connection.h"
#include "utils.h"
#include "Acceptor.h"
#include <functional>
#include <cstring>
#include <unistd.h>
#include <map>


Server::Server(EventLoop* _loop) : loop(_loop) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::new_connection, this, std::placeholders::_1);
    acceptor->set_new_connection_callback(cb);
}

Server::~Server() {
    delete acceptor;
}

// socket 由 Acceptor 传入，为客户端连接的 socket
void Server::new_connection(Socket *socket) {
    Connection* conn = new Connection(loop, socket, clients);
    std::function<void(Socket*)> cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
    conn->set_delete_connection_callback(cb);
    connections[socket->get_fd()] = conn;
    clients[socket->get_fd()] = {socket->get_fd(), ""};
}

void Server::delete_connection(Socket *socket) {
    auto conn = connections[socket->get_fd()];
    connections.erase(socket->get_fd());
    clients.erase(socket->get_fd());
    delete conn;
}