#include "include/Server.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <map>
#include <thread>

#include "include/Acceptor.h"
#include "include/Channel.h"
#include "include/Connection.h"
#include "include/EventLoop.h"
#include "include/InetAddress.h"
#include "include/Socket.h"
#include "include/ThreadPool.h"
#include "include/utils.h"

Server::Server(EventLoop* _main_reactor) : main_reactor(_main_reactor) {
  acceptor = new Acceptor(main_reactor);
  std::function<void(Socket*)> cb =
      std::bind(&Server::new_connection, this, std::placeholders::_1);
  acceptor->set_new_connection_callback(cb);

  int size = std::thread::hardware_concurrency();
  thread_pool = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    sub_reactors.push_back(new EventLoop());
  }
  for (int i = 0; i < size; ++i) {
    std::function<void()> sub_loop =
        std::bind(&EventLoop::loop, sub_reactors[i]);
    thread_pool->add(sub_loop);
  }
}

Server::~Server() {
  delete acceptor;
  delete thread_pool;
}

// socket 由 Acceptor 传入，传入客户端连接的 socket
void Server::new_connection(Socket* socket) {
  if (socket->get_fd() != -1) {
    int idx = socket->get_fd() % sub_reactors.size();
    Connection* conn = new Connection(sub_reactors[idx], socket, clients);
    std::function<void(Socket*)> cb =
        std::bind(&Server::delete_connection, this, std::placeholders::_1);
    conn->set_delete_connection_callback(cb);
    connections[socket->get_fd()] = conn;
    clients[socket->get_fd()] = {socket->get_fd(), ""};
  }
}

void Server::delete_connection(Socket* socket) {
  auto conn = connections[socket->get_fd()];
  connections.erase(socket->get_fd());
  clients.erase(socket->get_fd());
  delete conn;  // 可能 Segmentation fault
}