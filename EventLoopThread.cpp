#include "EventLoopThread.h"

EventLoopThread::EventLoopThread()
    : _loop(new EventLoop()),
      _thread(std::bind(&EventLoopThread::threadFunc, this))
{ 
    _thread.start();
}

EventLoopThread::~EventLoopThread()
{
    _loop->quit();
    _thread.join();
}

void EventLoopThread::threadFunc()
{

    EventLoop loop;
    _loop = &loop;
    loop.loop();
    _loop = NULL;
}
