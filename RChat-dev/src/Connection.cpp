#include "Connection.h"
#include "Socket.h"
#include "Server.h"
#include "Channel.h"
#include "EventLoop.h"
#include <functional>
#include <unistd.h>
#include <map>
#include "utils.h"
constexpr int READ_BUFFER = 1024;

Connection::Connection(EventLoop* _loop, Socket* _socket, std::map<int, Client>& clients) : loop(_loop), socket(_socket), client_map(clients), channel(nullptr) {
    channel = new Channel(loop, socket->get_fd());
    std::function<void()> cb = std::bind(&Connection::handle_readevent, this, socket->get_fd());
    channel->set_callback(cb);
    channel->enable_reading();
}

Connection::~Connection() {
    delete channel;
    delete socket;
}

void Connection::handle_readevent(int fd) {
    char buffer[READ_BUFFER];
    int read_bytes = read(fd, buffer, sizeof buffer);
    if (read_bytes < 0) {
        printf("ERROR: read error! client_sockfd = %d\n", fd);
        return;
    } else if (read_bytes == 0) {   // client disconnected
        printf("EOF, client_sockfd: %d disconnected!\n", fd);
        delete_connection_callback(socket);
    } else {
        std::string msg(buffer, read_bytes);
        if (client_map[fd].name == "") {       // msg is username if its username is empty
            client_map[fd].name = msg;
        } else {        // otherwise, it's a chat msg.
            for (auto &[client_sockfd, client]: client_map) {
                if (client_sockfd == fd) {        // send msg to everyone except itself
                    continue;
                }
                std::string msg_write = "[" + client_map[fd].name + "]: " + msg;
                write(client_sockfd, msg_write.c_str(), msg_write.size());
            }
        }
    }
}

void Connection::set_delete_connection_callback(std::function<void(Socket*)> _cb) {
    delete_connection_callback = _cb;
}
