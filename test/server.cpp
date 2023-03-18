// linux 服务端
#include "include/Server.h"
#include "include/EventLoop.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  loop->loop();
  return 0;
}