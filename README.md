# 简介
RChat 是一项完全采用 C++ 语言编写的 socket 网络编程项目。支持多人在线聊天。

技术点：基础面向对象、socket 编程、epoll 并发模式、简单多线程编程。

- 服务端开发环境：Ubuntu 20.04 
- 客户端开发环境：Ubuntu 20.04, Windows 10(优先级较低)

**目前开发重心迁移至扩展服务端功能，此项目暂停开发...**

# 版本简介
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

## version 0.2.0
> 2022.3.10

服务器与事件驱动核心类登场

服务器改造成 Reactor 模式。首先我们将整个服务器抽象成一个 Server 类，
这个类中有一个 main-Reactor（在这个版本没有 sub-Reactor ），里面的核心是一个 EventLoop（libevent 中叫做 EventBase ），
这是一个事件循环，我们添加需要监听的事务到这个事件循环内，每次有事件发生时就会通知（在程序中返回给我们 Channel ），
然后根据不同的描述符、事件类型进行处理（以回调函数的方式）。

*tips:如果文字看不懂，可以尝试跟着完成代码，会从中理解不少*

## version 0.2.1
> 2022.3.10

加入 Acceptor, Connection 类，TCP连接亦模块化，Reactor 模式逐渐成型

### bug修正
问题：
1. 维持多个客户端连接时，结束其中一个客户端连接，服务器异常终止运行，与其他客户端连接断开。
2. 加入完 Connection 类以后，出现了后一个连接（客户端）收不到前一个连接发送的消息，debug 发现，通过引用 map 在 `std::function` 中传递不具有时效性，
即前一个连接回调函数 `handle_readevent()` 中的参数是旧版本的 map，因此在前一个连接的函数中循环 `map<int, Client> clients` 不会找到新连接的 键值，
因此后者无法接受前者消息。

解决方法:
1. 在执行 `~Channel()` 时，不应加入 `delete loop` 使得事件驱动异常终止。
2. 在 Connection 类中加入绑定 `std::map<int, Client>` 类型引用的数据成员以共享此数据结构。

总结：
- 加强对整体框架的认识，各个类之间的联系。
- 对于 `std::function` 的原理掌握需要加强，其参数不具有时效性。
- 共享某个数据结构，可以通过其他类中存储这个数据结构的指针（引用）来实现，而不是通过 std::function 中的参数来传递。


## version 0.2.2
> 2022.3.11

加入 Buffer 类，设置 socket `SO_REUSEADDR`

## version 0.3.0
> 2022.3.13

加入线程池，添加线程支持任何函数作为线程函数。

### bug修正
问题：
- 客户端连接服务器之后，显示正常连接，服务器在 `Acceptor::accpet_connection()` 中 `socket->accept()` 之后的语句没有打印出来。客户端之间的消息也不能相互接收。

解决方法：
- 通过 `tcpdump port 端口号` 抓包发现服务器与客户端的确正常连接并且数据可以正常收发（没看服务器能不能将消息转发给客户端）但可以确定连接这块没有问题，猜测有可能是 Channel 或者 Connection 上出了问题。排查了一下bug发现是 Channel 的 `handle_event()` 函数调用的回调函数是空，原因是修改了一下回调的逻辑但是没有更换具体的回调函数，导致调用为空。

## version 0.3.1
> 2022.3.18

- 将服务器模式改为主从 Reactor 模式（多 Reactor，多线程）。
- 更改文件结构，头文件放入 `/src/include` 中，为 Cmake 做准备。









