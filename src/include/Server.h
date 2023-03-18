#pragma once
#include <map>
#include <vector>

class Socket;
class EventLoop;
struct Client;
class Acceptor;
class Connection;
class ThreadPool;

class Server {
 private:
  EventLoop *main_reactor;
  Acceptor *acceptor;  // 接受 TCP 连接
  std::map<int, Connection *> connections;
  std::map<int, Client> clients;
  std::vector<EventLoop *> sub_reactors;
  ThreadPool *thread_pool;

 public:
  Server(EventLoop *);
  ~Server();
  void handle_read_event(int sockfd);
  void new_connection(Socket *server_socket);
  void delete_connection(Socket *server_socket);
};