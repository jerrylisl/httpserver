#include "Handler.h"

Handler::Handler(const int connfd)
      :_connfd(connfd)
{
    _isClosed = false;
}

Handler::~Handler()
{
    if(!_isClosed)
        close(_connfd);
}

void Handler::handle()
{
    if(!receiveRequest())
    {
        close(_connfd);
        _isClosed = true;
        return;
    }
    if(_request.method != "GET" && _request.method != "POST")
    {
        sendErrorMsg("501", "Not Implemented",
             "Server doesn't implement this method");
        return;
    }
    parseURI();
    //auto res = checkFile();
    auto res = simpleCheckFile();
    if (res != OK)
        return;


    getFileType();
    std::string msg = "HTTP/1.1 200 OK\r\n";
    msg += "Server: Tiny Web Server\r\n";
    //msg += "Content-length: " + std::to_string(_contextLen) + "\r\n";
    msg += "Content-type: " + _fileType + "\r\n\r\n";
    _outputBuffer.append(msg.c_str(), msg.size());
    solveFile();
    if (_request.connection != "keep-alive")
    {
        if (close(_connfd) < 0)
            std::cout << "close error" << strerror(errno) << std::endl;
        _isClosed = true;
    }

}

void Handler::solveFile()
{
    if (_request.method == "POST")
        solvePost();
    else if (_request.method == "GET")
        solveGet();
    else if (_request.method == "PUT")
        solvePut();

}

bool Handler::receiveRequest()
{ 
    if(_inputBuffer.readFd(_connfd) == 0)
        return false;
    std::string request = _inputBuffer.readAllAsString();

    //std::cout << "---------------------------Request---------------------------" << std::endl;
    //std::cout << request << std::endl;
   // std::cout << "-------------------------------------------------------------" << std::endl;

    //std::cout << request << std::endl;
    Parser p(request);
    _request = p.getParseResult();
    _parameter = p.getParameter();
    //std::cout << p.getParserContext() << std::endl;
    return true;
}

void Handler::sendErrorMsg(const std::string &errorNum,
                  const std::string &shortMsg,
                  const std::string &longMsg)
{
    std::string msg = "HTTP /1.0 " + errorNum + " " + shortMsg + "\r\n";
    msg += "Content-type: text/html\r\n";
    msg += "<html><title>Error</title>";
    msg += "<body bgcolor=""ffffff"">\r\n";
    msg += errorNum + ": " + shortMsg + "\r\n";
    msg += "<p>" + longMsg + ": " + _fileName + "\r\n";
    msg += "<hr><em>The Tiny Web server</em>\r\n";
    _outputBuffer.append(msg.c_str(), msg.size());
    _outputBuffer.sendFd(_connfd);
    close(_connfd);
    _isClosed = true;
}

void Handler::parseURI()
{
    _wholeName = _root;
    if(_request.uri == "/")
        _fileName += "home.html";
    else
        _fileName = _request.uri.substr(1);
    _wholeName += _fileName;
}

Handler::FILESTAT Handler::checkFile()
{
    if (_request.uri == "/")
        _request.uri += "home";
    DIR *dp;                      // 定义子目录流指针
    struct dirent *entry;         // 定义dirent结构指针保存后续目录
    if((dp = opendir(_root.c_str())) == NULL) // 打开目录，获取子目录流指针，判断操作是否成功
    {
        std::cout << "can't open the root dirction" << std::endl;
        return DIRERR;
    }
    Handler::FILESTAT res = FOUNDERR;

    while((entry = readdir(dp)) != NULL)  // 获取下一级目录信息，如果未否则循环
    {
        if (strstr(entry->d_name, _fileName.c_str()) != NULL)
        {
            _fileName = entry->d_name;
            _wholeName = _root + _fileName;
        }
        if (strcmp(entry->d_name, _fileName.c_str()) == 0)
        {
            res = OK;
            struct stat fileInfo;
            //std::cout << _wholeName << std::endl;
            stat(_wholeName.c_str(), &fileInfo);
            if(!(S_ISREG(fileInfo.st_mode)) || !(S_IRUSR & fileInfo.st_mode))
            {
                res = MODEERR;
                break;
            }
            _fileSize = fileInfo.st_size;

            _request.uri = entry->d_name;

            if (_request.uri.find('.') != std::string::npos)
            {
                _requestFileType = _request.uri.substr(_request.uri.find('.') + 1);
                if (find(_types.begin(), _types.end(), _requestFileType) == _types.end())
                    res = TYPEERR;
            }
            else
                _requestFileType = "";
            break;
        }

    }
    closedir(dp);    // 关闭子目录流
    return res;
}

Handler::FILESTAT Handler::simpleCheckFile()
{
    struct stat fileInfo;
    if(stat(_wholeName.c_str(), &fileInfo) < 0)
    {
        std::cout << "404 Not found: Server couldn't find this file." << std::endl;
        sendErrorMsg("404", "Not Found", "Server couldn't find this file");
        return FOUNDERR;
    }
    if(!(S_ISREG(fileInfo.st_mode)) || !(S_IRUSR & fileInfo.st_mode))
    {
        std::cout << "403 Forbidden: Server couldn't read this file." << std::endl;
        sendErrorMsg("403", "Forbidden", "Server couldn't read this file");
        return MODEERR;
    }
    _fileSize = fileInfo.st_size;
    return OK;
}

void Handler::getFileType()
{
    const char *name = _fileName.c_str();
    if(strstr(name, ".html"))
        _fileType = "text/html";
    else if(strstr(name, ".pdf"))
        _fileType = "application/pdf";
    else if(strstr(name, ".png"))
        _fileType = "image/png";
    else if(strstr(name, ".gif"))
        _fileType = "image/gif";
    else if(strstr(name, ".jpg"))
        _fileType = "image/jpg";
    else if(strstr(name, ".jpeg"))
        _fileType = "image/jpeg";
    else if(strstr(name, ".css"))
        _fileType = "test/css";
    else
        _fileType = "text/html";
}

void Handler::solveGet()
{
    if (_requestFileType == "py")
        solvePy();
    else
        solveText();
}

void Handler::solvePost()
{
    if (_requestFileType == "py")
    {
        if (_parameter.empty())
            solvePy();
        else
            solvePywithParameter();
    }
    else
        solveText();
}

void Handler::solvePut()
{

}

void Handler::solveText()
{
    //struct stat fileInfo;
    //stat(_wholeName.c_str(), &fileInfo);
    int fd = open(_wholeName.c_str(), O_RDONLY, 0);
    //_contextLen = _outputBuffer.readFd(fd);
    //std::cout << _outputBuffer.readAllAsString() << std::endl;
    _outputBuffer.sendFd(_connfd);
    sendfile(_connfd, fd, NULL, _fileSize);
    if (close(fd) < 0)
        std::cout << "close error" << strerror(errno) << std::endl;
}

void Handler::solvePy()
{
    int p[2];
    pipe(p);
    std::string command = "python " + _wholeName;
    int old_fd = dup(STDOUT_FILENO);
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);
    system(command.c_str());
    dup2(old_fd, STDOUT_FILENO);
    close(old_fd);
    _contextLen = _outputBuffer.readFd(p[0]);
    //std::cout << _outputBuffer.readAllAsString() << std::endl;
    _outputBuffer.sendFd(_connfd);
    close(p[0]);
}

void Handler::solvePywithParameter()
{
    int p[2];
    pipe(p);
    std::string para = "\"";
    for (int i = 0; i < _parameter.size(); ++ i)
    {
        if (i != 0)
        para += '&';
        para += _parameter[i];
    }
    para += "\"";
    std::string command = "python " + _wholeName + " " + para;
    int old_fd = dup(STDOUT_FILENO);
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);
    system(command.c_str());
    dup2(old_fd, STDOUT_FILENO);
    close(old_fd);
    _contextLen = _outputBuffer.readFd(p[0]);
    //std::cout << _outputBuffer.readAllAsString() << std::endl;
    _outputBuffer.sendFd(_connfd);
    close(p[0]);
}

void Handler::solvePhp()
{
    int p[2];
    pipe(p);
    std::string command = "php " + _wholeName;
    int old_fd = dup(STDOUT_FILENO);
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);
    system(command.c_str());
    dup2(old_fd, STDOUT_FILENO);
    close(old_fd);
    _contextLen = _outputBuffer.readFd(p[0]);
    //std::cout << _outputBuffer.readAllAsString() << std::endl;
    _outputBuffer.sendFd(_connfd);
    close(p[0]);
}

void Handler::solvePhpwithParameter()
{
    int p[2];
    pipe(p);
    std::string para = "\"";
    for (int i = 0; i < _parameter.size(); ++ i)
    {
        if (i != 0)
        para += '&';
        para += _parameter[i];
    }
    para += "\"";
    std::string command = "php " + _wholeName + " " + para;
    int old_fd = dup(STDOUT_FILENO);
    dup2(p[1], STDOUT_FILENO);
    close(p[1]);
    system(command.c_str());
    dup2(old_fd, STDOUT_FILENO);
    close(old_fd);
    _contextLen = _outputBuffer.readFd(p[0]);
    //std::cout << _outputBuffer.readAllAsString() << std::endl;
    _outputBuffer.sendFd(_connfd);
    close(p[0]);
}

std::string Handler::makeHeader()
{
    std::string msg = "HTTP/1.1 200 OK\r\n";
    msg += "Server: Tiny Web Server\r\n";
    //msg += "Content-length: " + std::to_string(_contextLen) + "\r\n";
    msg += "Content-type: " + _fileType + "\r\n\r\n";
    return msg;
}

