#include "Socket.h"
#include "InetAddress.h"
#include "utils.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket(): fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd): fd(_fd) {
    errif(fd == -1, "socket create error");
}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr) {
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "bind error");
}

void Socket::listen() {
    errif(::listen(fd, SOMAXCONN), "listen error");
}

void Socket::set_nonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Socket::set_reuseaddr() {
    int opt = 1;
    int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    if (ret == -1) {
        close(fd);
        errif(true, "set socket reuseaddr failed");
    }
}

int Socket::accept(InetAddress *addr) {
    int client_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(client_sockfd == -1, "socket accpet error");
    return client_sockfd;
}

int Socket::get_fd() {
    return fd;
}


