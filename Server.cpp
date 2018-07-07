#include "Server.h"


std::string Handler::_root = "/home/jerry/page/";
std::string Parser::_tmp = Handler::_root + "tmp/";
std::vector<std::string> Handler::_types {"html", "py", "php", ""};
Server* Server::_server = NULL;
bool Server::_isStarted = false;

Server::Server(std::string path)
    : _loader(path)

{
    if (_server == NULL)
    {
        _server = this;
        _isStarted = true;
    }
    else
    {
        std::cout << "already started" << std::endl;
        exit(1);
    }
    auto res = setSigHandler();
    if (res)
    {
        std::cout << "signalHandler set error" << std::endl;
        exit(1);
    }
    _listenFd = Socket::createSocket();
    if (_loader.LoadingOption())
        exit(1);
    if (_loader.getRoot() != "")
    {
        Handler::_root = _loader.getRoot();
        Parser::_tmp = Handler::_root + "tmp/";
    }
    if (!_loader.getFileTypes().empty())
    {
        Handler::_types = _loader.getFileTypes();
        Handler::_types.push_back("");
    }

    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _servAddr.sin_port = htons(_loader.getPort());
    Socket::Bind(_listenFd, _servAddr);
    std::cout << Handler::_root << std::endl;
}

void Server::start()
{
    Socket::Listen(_listenFd);
    _threadPool = new EventLoopThreadPool(_loader.getThreadNum());
    while(true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        memset(&clientAddr, 0, sizeof(clientAddr));
        int connFd = Socket::Accept(_listenFd, &clientAddr);

        // 挑选一个线程，将已连接套接字注册到此线程的EventLoop中
        EventLoopThread *thread = _threadPool->getNextThread();
        EventLoop *loop = thread->getLoop();
        loop->addToLoop(connFd);
    }
}

Server::~Server()
{
    std::cout << "Close Server" << std::endl;
    Socket::Close(_listenFd);
    delete _threadPool;
    clearTmp();
}

void Server::clearTmp()
{
    DIR *dp;                      // 定义子目录流指针
    struct dirent *entry;         // 定义dirent结构指针保存后续目录
    if((dp = opendir(Parser::_tmp.c_str())) == NULL) // 打开目录，获取子目录流指针，判断操作是否成功
    {
        std::cout << "can't open the tmp dirction" << std::endl;
    }

    while((entry = readdir(dp)) != NULL)  // 获取下一级目录信息，如果未否则循环
    {

        if (entry->d_type ==   DT_REG)
        {
            std::string removeTemp = Parser::_tmp + entry->d_name;
            unlink(removeTemp.c_str());
        }
    }
    closedir(dp);    // 关闭子目录流
}

void Server::handler(int sig)
{
    _server->~Server();
    signal(sig, SIG_DFL);
    raise(sig);
}











