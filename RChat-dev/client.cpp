#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "utils.h"

constexpr int BUF_SIZE = 1024;


void* send_msg(void* arg) {
    int sockfd = *((int*)arg);
    char msg[BUF_SIZE];
    while (true) {
        scanf("%s", msg);
        if (!strcmp(msg, "QUIT") || !strcmp(msg, "quit")) {
            close(sockfd);
            exit(0);
        }
        send(sockfd, msg, sizeof(msg), 0);
    }
}

void* recv_msg(void* arg) {
    int sockfd = *((int*)arg);
    char msg[BUF_SIZE];
    while (true) {
        ssize_t read_bytes = recv(sockfd, msg, sizeof(msg) - 1, 0);
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
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    // 绑定ip与端口
    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6183);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // serv_addr.sin_addr.s_addr = inet_addr("172.23.125.118");

    printf("Connecting...\n");
    
    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "connect error");
    printf("欢迎来到聊天室，请输入您的聊天用户名：");
    
    // 双线程，一个循环发消息，一个循环收消息。
    pthread_t send_tid, recv_tid;
    pthread_create(&send_tid, NULL, send_msg, (void*)&sockfd);
    pthread_create(&recv_tid, NULL, recv_msg, (void*)&sockfd);

    pthread_join(send_tid, NULL);
    pthread_join(recv_tid, NULL);

    close(sockfd);
    return 0;
}