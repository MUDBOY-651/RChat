src=$(wildcard src/*.cpp)

server:
	g++ -std=c++17 -pthread -g \
	$(src) \
	server.cpp \
	-o server
	
client:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp client.cpp -std=c++17 -o client

# th:
#	g++ -pthread src/ThreadPool.cpp ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp src/ThreadPool.cpp \
	-pthread -std=c++17 \
	test.cpp -o test

clean:
	rm server && rm client && rm test
