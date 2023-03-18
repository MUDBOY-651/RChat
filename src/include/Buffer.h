#pragma once
#include <string>
#include "Macros.h"

class Buffer {
 private:
  std::string buf;

 public:
  Buffer();
  ~Buffer();
  DISALLOW_COPY_AND_MOVE(Buffer);
  void append(const char* s, int len);
  ssize_t size();
  const char* c_str();
  void clear();
  void getline();
  std::string get_buf();
  void set_buf(const char* _buf);
};
