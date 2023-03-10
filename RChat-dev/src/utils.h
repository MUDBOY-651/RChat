#pragma once
#include <string>

// 错误处理函数
void errif(bool, const char*);

// 一个客户端的标识：{sockfd, 用户名}
struct Client {
    int sockfd;
    std::string name;
};