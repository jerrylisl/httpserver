#ifndef SERVER
#define SERVER

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Socket.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "EventLoopThreadPool.h"
#include "Optionloader.h"
#include "Handler.h"
#include "Parser.h"

class Server
{
public:
    explicit Server(std::string path);

    void start();

    bool setSigHandler()
    {
        auto res = signal(SIGINT, handler);
        if (res == SIG_ERR)
            return 1;
        return 0;
    }

    ~Server();

    static void notifyServer(std::string);

private:
    static void handler(int sig);
    static bool _isStarted;
    static Server* _server;

    void clearTmp();


    OptionLoader _loader;
    int _listenFd;
    sockaddr_in _servAddr;
    EventLoopThreadPool * _threadPool;
};



#endif // SERVER

