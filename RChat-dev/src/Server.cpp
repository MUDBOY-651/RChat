#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "utils.h"
#include <functional>
#include <cstring>
#include <unistd.h>
#include <map>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop* _loop) : loop(_loop) {
    Socket *server_socket = new Socket();
    InetAddress *server_address = new InetAddress(3389);
    server_socket->bind(server_address);
    server_socket->listen();

    Channel *server_channel = new Channel(loop, server_socket->get_fd());
    std::function<void()> cb = std::bind(&Server::new_connection, this, server_socket);
    server_channel->set_callback(cb);
    server_channel->enable_reading();
}

Server::~Server() {
}

void Server::handle_read_event(int fd) {
    char buffer[READ_BUFFER];
    int read_bytes = read(fd, buffer, sizeof buffer);
    if (read_bytes < 0) {
        printf("ERROR: read error! client_sockfd = %d\n", fd);
        return;
    } else if (read_bytes == 0) {   // client disconnected
        printf("EOF, client_sockfd: %d disconnected!\n", fd);
        close(fd);
        clients.erase(fd);
    } else {
        std::string msg(buffer, read_bytes);
        if (clients[fd].name == "") {       // msg is username if its username is empty
            clients[fd].name = msg;
        } else {        // otherwise, it's a chat msg.
            for (auto &[client_sockfd, client]: clients) {
                if (client_sockfd == fd) {        // send msg to everyone except itself
                    continue;
                }
                std::string msg_write = "[" + clients[fd].name + "]: " + msg;
                write(client_sockfd, msg_write.c_str(), msg_write.size());
            }
        }
    }
}

void Server::new_connection(Socket *server_socket) {
    InetAddress *client_addr = new InetAddress();   // 内存泄露
    Socket *client_socket = new Socket(server_socket->accept(client_addr));     // 内存泄露
    printf("new client connected! fd: %d, IP: %s, Port: %d\n", 
        client_socket->get_fd(), inet_ntoa(client_addr->addr.sin_addr), ntohs(client_addr->addr.sin_port));
    Channel *client_channel = new Channel(loop, client_socket->get_fd());
    std::function<void()> cb = std::bind(&Server::handle_read_event, this, client_socket->get_fd());
    client_channel->set_callback(cb);
    client_channel->enable_reading();
    clients[client_channel->get_fd()] = {client_channel->get_fd(), ""};
}