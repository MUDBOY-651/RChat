#pragma once
#include <map>

class Socket;
class EventLoop;
struct Client;
class Acceptor;

class Server {
private:
    EventLoop* loop;
    Acceptor *acceptor;
    std::map<int, Client> clients;
public:
    Server(EventLoop*);
    ~Server();
    void handle_read_event(int sockfd);
    void new_connection(Socket *server_socket);
};