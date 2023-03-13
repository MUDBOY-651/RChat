#include "Buffer.h"
#include <iostream>

Buffer::Buffer() {
}

Buffer::~Buffer() {
}

void Buffer::append(const char* s, int len) {
    for (int i = 0; i < len; i++) {
        if (s[i] == '\0')
            break;
        buf.push_back(s[i]);
    }
}

ssize_t Buffer::size() {
    return buf.size();
}

const char* Buffer::c_str() {
    return buf.c_str();
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getline() {
    buf.clear();
    std::getline(std::cin, buf);
}

std::string Buffer::get_buf() {
    return buf;
}

void Buffer::set_buf(const char* _buf) {
    buf.clear();
    buf.append(_buf);
}
