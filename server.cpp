// linux 服务端
#include "src/Server.h"
#include "src/EventLoop.h"

int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}