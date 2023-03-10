// linux 服务端
#include "src/Server.h"
#include "src/EventLoop.h"

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}

/*
int main() {
    Socket *server_socket = new Socket();
    InetAddress *server_address = new InetAddress(3389);
    server_socket->set_reuseaddr();
    server_socket->bind(server_address);
    server_socket->listen();
    // 保存客户端信息
    std::map<int, Client> clients;
    Epoll* ep = new Epoll();
    // ep->add_fd(server_socket->get_fd(), EPOLLIN);
    Channel *server_channel = new Channel(ep, server_socket->get_fd());
    server_channel->enable_reading();

    printf("Waiting for connection...\n");
    while (true) {
        auto evs = ep->poll();
        int nfds = evs.size();
        for (int i = 0; i < nfds; ++i) {
            int fd = evs[i]->get_fd();
            // 监听的 socket 收到消息，表明有客户端进行连接了。
            if (fd == server_socket->get_fd()) {
                InetAddress* client_addr = new InetAddress();
                Socket* client_socket = new Socket(server_socket->accept(client_addr));
                printf("new client connected! fd: %d, IP: %s, Port: %d\n", 
                        client_socket->get_fd(), inet_ntoa(client_addr->addr.sin_addr), ntohs(client_addr->addr.sin_port));

                Channel *client_channel = new Channel(ep, client_socket->get_fd());
                client_channel->enable_reading();

                clients[client_socket->get_fd()] = {client_socket->get_fd(), ""};
            } else if (evs[i]->get_revents() & EPOLLIN) {        
                // 客户端有消息，则代表客户端发送了消息过来，保存并发给其他客户端。
                char buffer[BUF_SIZE];
                int read_bytes = read(fd, buffer, sizeof buffer);
                if (read_bytes < 0) {
                    printf("ERROR: read error! client_sockfd = %d\n", fd);
                    break;
                } else if (read_bytes == 0) {   // client disconnected
                    ep->del_fd(fd);
                    clients.erase(fd);
                } else {
                    std::string msg(buffer, read_bytes);
                    if (clients[fd].name == "") {       // msg is username if its username is empty
                        clients[fd].name = msg;
                    } else {        // otherwise, it's a chat msg.
                        for (auto &[client_sockfd, client]: clients) {
                            if (client_sockfd == fd) {        // send msg to everyone except it 
                                continue;
                            }
                            std::string msg_write = "[" + clients[fd].name + "]: " + msg;
                            write(client_sockfd, msg_write.c_str(), msg_write.size());
                        }
                    }
                }
            } else {
                puts("Something else happened");
                // other events, need to implement later;
            }
        }
    }
    delete ep;
    delete server_socket;
    return 0;
}
*/