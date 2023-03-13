#pragma once
#include<string>

class Buffer {
private:
    std::string buf;
public:
    Buffer();
    ~Buffer();
    void append(const char* s, int len);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
    std::string get_buf();
    void set_buf(const char* _buf);
};
