#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <assert.h>
#include <vector>
#include <poll.h>
#include <signal.h>
#include "Epoll.h"
#include "CurrentThread.h"
#include"Mutex.h"

class Handler;
class EventLoop : Noncopyable
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void addToLoop(const int fd);

private:
    void addToLoop();
    bool isLooping;
    const pid_t threadId;
    std::vector<int> fds;
    bool isQuit;
    Epoll *e;
    Mutex _mutex;
};

#endif // EVENTLOOP_H
