# 简介
RChat 是一项完全采用 C++ 语言编写的 socket 网络编程项目。支持多人在线聊天。

技术点：基础面向对象、socket 编程、epoll 并发模式、简单多线程编程。

- 服务端开发环境：Ubuntu 20.04 
- 客户端开发环境：Ubuntu 20.04, Windows 10(优先级较低)

# 版本迭代
## version 0.1.0
> 2022.3.2

经过了一系列的 debug，最终莫名解决了所有 bug，项目能够实现最基本的功能。但代码还存在一定的缺陷(代码逻辑重复，没有模块化等..)。
### 展示
![1.0-1](https://img2023.cnblogs.com/blog/2500770/202303/2500770-20230302142928485-387769446.png)

![1.0-2](https://img2023.cnblogs.com/blog/2500770/202303/2500770-20230302142936586-1988041157.png)

### bug 修正
- 服务端和客户端建立连接的bug解决是一件很神奇的事情，无法描述，个人猜测是端口只能使用云服务器安全组开放的端口。
- 在消息传递方面的bug是一些低级的逻辑错误:
    - 客户端的消息输入应该采用 `getline(std::cin, msg)` 而非 `std::cin` 前者读入空格以回车结尾。
    - 向客户端传输消息 `[username]:msg` username 为 `clients[sockfd].name` 代码逻辑错误，很低级。

## version 0.1.1
> 2022.3.2

编写了若干个类简化服务端 main 函数代码，将整个项目代码模块化。隐藏了底层语言实现细节、增加了可读性。

目录结构：
```
client.cpp
Epoll.cpp
Epoll.h
InetAddress.cpp
InetAddress.h
Makefile
server.cpp
Socket.cpp
Socket.h
utils.cpp
utils.h
```

## version 0.1.2
> 2022.3.8

加入 `Channel` 类

### bug 修正
在向内核事件表中加入 监听socket 事件的代码中未创建 `server_channel`，导致其为未定义，访问对应的内存区域产生了 `Segmentation Fault` 报错。








