#ifndef HANDLER_H
#define HANDLER_H

#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "Parser.h"
#include "Buffer.h"


class Handler 
{
public:
    enum FILESTAT {OK, DIRERR, FOUNDERR, MODEERR, TYPEERR};


    Handler(const int connfd);
    ~Handler();
    void handle();
    const int connFd() const
    {
        return _connfd;
    }
    void solveFile();

    static std::string _root;
    static std::string _tmp;  //临时存储目录，对put的支持
    static std::vector<std::string> _types;
private:
    bool receiveRequest();  // 接受客户的请求并解析
    void sendResponse();    // 发送相应
    void sendErrorMsg(const std::string &errorNum,
                      const std::string &shortMsg,
                      const std::string &longMsg);

    void parseURI();
    FILESTAT checkFile();
    FILESTAT simpleCheckFile();

    void getFileType();

    void solveGet();

    void solvePost();

    void solveText();
    void solvePy();
    void solvePywithParameter();

    int _connfd;
    bool _isClosed;
    std::string _fileType; //这个是应答报文的正文内容
    std::string _fileName; //文件名，用于搜索
    std::string _wholeName;  //全路径，用于访问
    std::string _requestFileType;  //这个是请求文件的文件类型
    Buffer _inputBuffer;
    Buffer _outputBuffer;
    HTTPRequest _request;
    int _contextLen;
    std::string _context;
    std::vector<std::string> _parameter;
    std::vector<std::string> _updateFile; //put时，先将文件保存在临时目录


};



#endif // HANDLER_H
