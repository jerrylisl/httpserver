#ifndef MUTEX_H
#define MUTEX_H

#include<pthread.h>
#include"Base.h"

class Mutex : Noncopyable
{
public:
    Mutex() 
    {
        pthread_mutex_init(&m_mutex, NULL);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    pthread_mutex_t& getPthreadMutex()
    {
        return m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};

class MutexLock
{
public:
    explicit MutexLock(Mutex &m): m_mutex(m)
    {
        m_mutex.lock();
    }

    ~MutexLock()
    {
        m_mutex.unlock();
    }

private:
    Mutex& m_mutex;
};

#endif // MUTEX_H
