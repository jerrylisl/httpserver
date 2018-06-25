#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "Handler.h"

class EventLoop;

class Epoll : Noncopyable
{
public:
    explicit Epoll(EventLoop *loop)
        :ownerLoop(loop),
         epollfd(epoll_create1(EPOLL_CLOEXEC)),
         retEvents(8) // 初始时为epoll_wait预留8个返回的epoll_event
    { 
        if(epollfd < 0)
            std::cout << "Epoll::epoll_create1() error: " << ::strerror(errno) << std::endl;
        assert(epollfd > 0);
    }

    ~Epoll()
    { close(epollfd); }

    // 调用epoll_wait，并将其交给Event类的handleEvent函数处理
    void epoll(std::vector<Handler*> &events);
    void removeFd(const int fd);
    void addToEpoll(const int fd);

private: 
    EventLoop *ownerLoop;
    int epollfd;
    std::vector<struct epoll_event> retEvents;
};

#endif // EPOLL_H
