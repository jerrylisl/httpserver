#define TEST


#include <iostream>
#include <cstdlib>
#include<string>
#include<vector>
#include "Socket.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "EventLoopThreadPool.h"
#include "Optionloader.h"

std::string Handler::_root = "/home/jerry/page/";
std::string Parser::_tmp = "/home/jerry/page/tmp/";

std::vector<std::string> Handler::_types {"", "html", "py"};


int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <optionfilepath>" << std::endl;
        return 0;
    }
    std::string path = argv[1];

    std::cout << "Start" << std::endl;

    OptionLoader loader(path);
    if (loader.LoadingOption())
        exit(1);

    if (loader.getRoot() != "")
    {
        Handler::_root = loader.getRoot();
        Parser::_tmp = Handler::_root + "tmp/";
    }
    if (!loader.getFileTypes().empty())
    {
        Handler::_types = loader.getFileTypes();
        Handler::_types.push_back("");
    }

    std::cout << Handler::_root << std::endl;








    
    int listenFd = Socket::createSocket();
    Socket::setReuseAddr(listenFd, true);
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(loader.getPort());
    Socket::Bind(listenFd, servAddr);
    Socket::Listen(listenFd);

    EventLoopThreadPool *threadPool = new EventLoopThreadPool(1);
    while(true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        memset(&clientAddr, 0, sizeof(clientAddr));
        int connFd = Socket::Accept(listenFd, &clientAddr);

        // 挑选一个线程，将已连接套接字注册到此线程的EventLoop中
        EventLoopThread *thread = threadPool->getNextThread();
        EventLoop *loop = thread->getLoop();
        loop->addToLoop(connFd);
    }
    Socket::Close(listenFd);
    delete threadPool;
    return 0;
}
