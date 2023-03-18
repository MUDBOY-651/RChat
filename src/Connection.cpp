#include "include/Connection.h"

#include <unistd.h>

#include <cstring>
#include <functional>
#include <map>

#include "include/Buffer.h"
#include "include/Channel.h"
#include "include/EventLoop.h"
#include "include/Server.h"
#include "include/Socket.h"
#include "include/utils.h"
constexpr int READ_BUFFER = 1024;

Connection::Connection(EventLoop* _loop, Socket* _socket,
                       std::map<int, Client>& clients)
    : loop(_loop), socket(_socket), client_map(clients), channel(nullptr) {
  channel = new Channel(loop, socket->get_fd());
  std::function<void()> cb =
      std::bind(&Connection::handle_readevent, this, socket->get_fd());
  channel->set_read_callback(cb);
  channel->enable_reading();
  read_buffer = new Buffer();
}

Connection::~Connection() {
  delete channel;
  delete socket;
  delete read_buffer;
}

void Connection::handle_readevent(int fd) {
  char buffer[READ_BUFFER];
  int read_bytes = read(fd, buffer, sizeof buffer);
  if (read_bytes < 0) {
    printf("ERROR: read error! client_sockfd = %d\n", fd);
    return;
  } else if (read_bytes == 0) {  // client disconnected
    printf("EOF, client_sockfd: %d disconnected!\n", fd);
    delete_connection_callback(socket);
  } else if (read_bytes > 0) {
    read_buffer->append(buffer, read_bytes);
    // msg is username if its username is empty
    if (client_map[fd].name == "") {
      client_map[fd].name = read_buffer->c_str();
    } else {  // otherwise, it's a chat msg.
      std::string msg_write =
          "[" + client_map[fd].name + "]: " + read_buffer->get_buf();
      for (auto& [client_sockfd, client] : client_map) {
        // send msg to everyone except itself
        if (client_sockfd == fd) {
          continue;
        }
        write(client_sockfd, msg_write.c_str(), msg_write.size());
      }
    }
    read_buffer->clear();
  }
}

void Connection::set_delete_connection_callback(
    std::function<void(Socket*)> _cb) {
  delete_connection_callback = _cb;
}

void Connection::send(int sockfd) {
  char buf[read_buffer->size()];
  strcpy(buf, read_buffer->c_str());
  int data_size = read_buffer->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    data_left -= bytes_write;
  }
}
