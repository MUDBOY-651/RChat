server:
	g++ server.cpp src/Acceptor.cpp src/Server.cpp src/EventLoop.cpp src/utils.cpp src/Epoll.cpp src/Socket.cpp src/InetAddress.cpp src/Channel.cpp -std=c++17 -D DBG -o server
client:
	g++ client.cpp src/utils.cpp -std=c++17 -D DBG -o client
clean:
	rm server && rm client
