#pragma once
#include <functional>
#include <map>

#include "Macros.h"

struct Client;
class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection {
 public:
  Connection(EventLoop *loop, Socket *socket, std::map<int, Client> &);
  ~Connection();
  DISALLOW_COPY_AND_MOVE(Connection);

  void handle_readevent(int fd);
  void set_delete_connection_callback(std::function<void(Socket *)> _cb);
  void send(int sockfd);

 private:
  EventLoop *loop;
  Socket *socket;
  Channel *channel;
  std::map<int, Client> &client_map;
  std::function<void(Socket *)> delete_connection_callback;
  Buffer *read_buffer;
};