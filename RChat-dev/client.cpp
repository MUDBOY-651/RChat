#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <thread>
#include "src/utils.h"
#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/Buffer.h"

constexpr int BUF_SIZE = 1024;


void* send_msg(int sockfd, Buffer* send_buffer) {
    while (true) {
        send_buffer->getline();
        if (send_buffer->get_buf() == "QUIT" || send_buffer->get_buf() == "quit") {
            close(sockfd);
            exit(0);
        }
        send(sockfd, send_buffer->c_str(), send_buffer->size(), 0);
    }
}

void* recv_msg(int sockfd, Buffer* read_buffer) {
    char msg[BUF_SIZE];
    while (true) {
        bzero(&msg, sizeof(msg));
        ssize_t read_bytes = recv(sockfd, msg, sizeof(msg), 0);
        if (read_bytes > 0) {
            msg[read_bytes] = '\0';
            puts(msg);
        } else if (read_bytes == 0) {
            puts("server socket disconnected!");
            continue;
        } else if (read_bytes == -1) {
            close(sockfd);
            return nullptr;
        }
    }
}

int main() {
    Socket* sock = new Socket();
    InetAddress* addr = new InetAddress("120.24.49.67", 3389);
    printf("Connecting...\n");
    sock->connect(addr);
    int sockfd = sock->get_fd();
    Buffer *read_buffer = new Buffer();
    Buffer *send_buffer = new Buffer();
    printf("欢迎来到聊天室，请输入您的聊天用户名：");
    std::thread send_thread(send_msg, sockfd, send_buffer), recv_thread(recv_msg, sockfd, recv_msg);
    send_thread.join();
    recv_thread.join();
    delete addr;
    delete sock;
    return 0;
}

// int main2() {
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     errif(sockfd == -1, "socket create error");

//     // 绑定ip与端口
//     sockaddr_in serv_addr;
//     bzero(&serv_addr, sizeof serv_addr);
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(3389);
//     serv_addr.sin_addr.s_addr = inet_addr("120.24.49.67");

//     printf("Connecting...\n");
    
//     errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "connect error");
//     printf("欢迎来到聊天室，请输入您的聊天用户名：");
    
//     // 双线程，一个循环发消息，一个循环收消息。
//     pthread_t send_tid, recv_tid;
//     pthread_create(&send_tid, NULL, send_msg, (void*)&sockfd);
//     pthread_create(&recv_tid, NULL, recv_msg, (void*)&sockfd);

//     pthread_join(send_tid, NULL);
//     pthread_join(recv_tid, NULL);

//     close(sockfd);
//     return 0;
// }