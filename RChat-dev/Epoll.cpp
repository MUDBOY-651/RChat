#include "Epoll.h"
#include "utils.h"
#include <unistd.h>
#include <cstring>

#define MAX_EVENTS 1024

Epoll::Epoll(): epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

void Epoll::add_fd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}

void Epoll::del_fd(int fd) {
    close(fd);
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0) == -1, "epoll delete event error");
}

std::vector<epoll_event> Epoll::poll(int timeout) {
    std::vector<epoll_event> activeEvent;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        activeEvent.push_back(events[i]);
    }
    return activeEvent;
}

int Epoll::get_fd() {
    return epfd;
}