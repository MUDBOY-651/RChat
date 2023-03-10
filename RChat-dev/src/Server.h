#pragma once
#include <map>

class Socket;
class EventLoop;
struct Client;
class Acceptor;
class Connection;

class Server {
private:
    EventLoop* loop;
    Acceptor *acceptor; // 接受 TCP 连接
    std::map<int, Connection*> connections;
    std::map<int, Client> clients;
public:
    Server(EventLoop*);
    ~Server();
    void handle_read_event(int sockfd);
    void new_connection(Socket *server_socket);
    void delete_connection(Socket *server_socket);
};