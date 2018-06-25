#include "EventLoop.h"

class IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        signal(SIGPIPE, SIG_IGN);
    }
};


IgnoreSigPipe initObj;

EventLoop::EventLoop()
    :isLooping(false),
     threadId(CurrentThread::gettid()),
     isQuit(false),
     e(new Epoll(this))
{ }

EventLoop::~EventLoop()
{
    assert(!isLooping);
}

void EventLoop::loop()
{
    assert(!isLooping);
    isLooping = true;
    isQuit = false;

    while(!isQuit)
    {
#ifdef TEST
        std::cout << "----------Looping----------" << std::endl;
#endif
        addToLoop();
        std::vector<Handler*> activeEvents;
        activeEvents.clear();
        e->epoll(activeEvents);
        for(std::vector<Handler*>::iterator iter = activeEvents.begin();
            iter != activeEvents.end(); ++iter)
        {
#ifdef TEST
            std::cout << "----------Handle request----------" << std::endl;
#endif
            (*iter)->handle();
            e->removeFd((*iter)->connFd());
            delete *iter;
        }
    }
    isLooping = false;
}

void EventLoop::quit()
{
    isQuit = true;
}


void EventLoop::addToLoop(const int fd)
{
#ifdef TEST
    std::cout << "----------Add " << fd << " to loop----------" << std::endl;
#endif
    {
        MutexLock lock(_mutex);
        fds.push_back(fd);
    }
}


void EventLoop::addToLoop()
{
    if(fds.empty())
    {
#ifdef TEST
        std::cout << "----------fds empty----------" << std::endl;
#endif
        return;
    }
    {
        MutexLock lock(_mutex);
        for(int i = 0; i < fds.size(); ++i)
            e->addToEpoll(fds[i]);
        fds.clear();
    }
#ifdef TEST
    std::cout << "----------Add all fd to loop----------" << std::endl;
#endif
}
