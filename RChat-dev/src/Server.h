#pragma once
#include <map>

class EventLoop;
class Socket;
struct Client;

class Server {
private:
    EventLoop* loop;
    std::map<int, Client> clients;
public:
    Server(EventLoop*);
    ~Server();
    void handle_read_event(int sockfd);
    void new_connection(Socket *server_socket);
};