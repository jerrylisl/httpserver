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
    if(_request.method != "GET")
    {
        sendErrorMsg("501", "Not Implemented",
             "Server doesn't implement this method");
        return;
    }
    parseURI();
    struct stat fileInfo;
    if(stat(_fileName.c_str(), &fileInfo) < 0)
    {
#ifdef TEST
        std::cout << "404 Not found: Server couldn't find this file." << std::endl;
#endif
        sendErrorMsg("404", "Not Found", "Server couldn't find this file");
        return;
    }
    if(!(S_ISREG(fileInfo.st_mode)) || !(S_IRUSR & fileInfo.st_mode))
    {
#ifdef TEST
        std::cout << "403 Forbidden: Server couldn't read this file." << std::endl;
#endif
        sendErrorMsg("403", "Forbidden", "Server couldn't read this file");
        return;
    }
    getFileType();
    std::string msg = "HTTP/1.1 200 OK\r\n";
    msg += "Server: Tiny Web Server\r\n";
    msg += "Content-length: " + std::to_string(fileInfo.st_size) + "\r\n";
    msg += "Content_type: " + _fileType + "\r\n\r\n";
    _outputBuffer.append(msg.c_str(), msg.size());
    solveFile();
    if (close(_connfd) < 0)
        std::cout << "close error" << strerror(errno) << std::endl;
    _isClosed = true;

}

void Handler::solveFile()
{
    if (_fileType == "text/html" ||
            _fileType == "application/pdf" ||
            _fileType == "image/png" ||
            _fileType == "image/gif" ||
            _fileType == "image/jpg" ||
            _fileType == "image/jpeg" ||
            _fileType ==  "test/css" ||
            _fileType == "text/plain")
        solveText();

}

bool Handler::receiveRequest()
{ 
    if(_inputBuffer.readFd(_connfd) == 0)
        return false;
    std::string request = _inputBuffer.readAllAsString();
#ifdef TEST
    std::cout << "---------------------------Request---------------------------" << std::endl;
    std::cout << request << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
#endif
    Parser p(request);
    _request = p.getParseResult();
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
    _fileName = root;
    if(_request.uri == "/")
        _fileName += "/home.html";
    else
        _fileName += _request.uri;
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
        _fileType = "text/plain";
}

void Handler::solveText()
{
    int fd = open(_fileName.c_str(), O_RDONLY, 0);
    _outputBuffer.readFd(fd);
    _outputBuffer.sendFd(_connfd);
    if (close(fd) < 0)
        std::cout << "close error" << strerror(errno) << std::endl;
}

