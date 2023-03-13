#include "Epoll.h"
#include "Channel.h"
#include "utils.h"
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdio>

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

std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> active_channels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*)events[i].data.ptr; 
        ch->set_revents(events[i].events);
        active_channels.push_back(ch);
        // activeEvent.push_back(events[i]);
    }
    return active_channels;
}

// 更新 Channel 类，添加事件到内核事件表。
void Epoll::update_channel(Channel* ch) {
    int fd = ch->get_fd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = ch;
    ev.events = ch->get_events();
    if (!ch->get_in_epoll()) {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        ch->set_in_epoll();
    } else {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

int Epoll::get_fd() {
    return epfd;
}

void Epoll::delete_channel(Channel *channel) {
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, channel->get_fd(), NULL) == -1, "epoll delete error");
    channel->set_in_epoll(false);
}