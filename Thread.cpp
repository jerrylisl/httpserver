#include <pthread.h>
#include <assert.h>
#include "Thread.h"

static void* startThread(void* obj)
{
    using ThreadFunc = std::function<void()>;
    ThreadFunc *f = static_cast<ThreadFunc*>(obj);
    (*f)();
    return NULL;
}


void Thread::start()
{
    using ThreadFunc = std::function<void()>;
    assert(!started);
    started = true;
    if(pthread_create(&pthreadId, NULL, startThread, &threadFunc))
        started = false;
#ifdef TEST
    std::cout << "----------Thread created----------" << std::endl;
#endif
}

int Thread::join()
{
    assert(started && !joined);
    joined = true;
    return pthread_join(pthreadId, NULL);
}
