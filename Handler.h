#ifndef HANDLER_H
#define HANDLER_H

#include <string>
#include <algorithm>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Parser.h"
#include "Buffer.h"

const std::string root = "/home/jerry/page";

class Handler 
{
public:
    Handler(const int connfd);
    ~Handler();
    void handle();
    const int connFd() const
    {
        return _connfd;
    }
    void solveFile();
private:
    bool receiveRequest();  // 接受客户的请求并解析
    void sendResponse();    // 发送相应
    void sendErrorMsg(const std::string &errorNum,
                      const std::string &shortMsg,
                      const std::string &longMsg);
    void parseURI();
    void getFileType();

    void solveText();

    int _connfd;
    bool _isClosed;
    std::string _fileType;
    std::string _fileName;
    Buffer _inputBuffer;
    Buffer _outputBuffer;
    HTTPRequest _request;
};

#endif // HANDLER_H
