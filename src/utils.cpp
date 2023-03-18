#include <unistd.h>

#include <cstdio>
#include <cstdlib>

void errif(bool condition, const char* msg) {
  if (condition) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}