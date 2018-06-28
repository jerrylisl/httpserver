#include "Parser.h"

Parser::Parser(const std::string& request)
    : _hasSeparator(false)
{
    assert(request.size() > 0);
    this->_request = request;
}

HTTPRequest Parser::getParseResult()
{
    parseLine();
    parseRequestLine();
    parseHeaders();
    if (_hasSeparator)
        parsePostWithSeparator();
    return _parseResult;
}

void Parser::parseLine()
{
    std::string::size_type lineBegin = 0;   // 正在解析的行的行首索引
    std::string::size_type checkIndex = 0;  // 正在解析的字符索引
    //std::cout << _request << std::endl;
    bool head = true;
    while(head && checkIndex < _request.size())
    {
        // 如果当前字符是'\r'，则有可能读到一行
        if(_request[checkIndex] == '\r')
        {
            // 如果当前字符是最后一个字符则说明请求没有读取完整
            if((checkIndex + 1) == _request.size())
            {
                std::cout << "Request not to read the complete." << std::endl;
                return;
            }
            // 如果下一个字符是'\n'，则说明我们读取到了完整的一行
            else if(_request[checkIndex+1] == '\n')
            {
                _lines.push_back(std::string(_request, lineBegin,
                            checkIndex - lineBegin));
                if (checkIndex == lineBegin)
                    head = false;
                checkIndex += 2;
                lineBegin = checkIndex;
            }
            else
            {
                std::cout << "Request error." << std::endl;
                return;
            }
        }
        else
            ++checkIndex;
    }
    if (checkIndex < _request.size())
        _context = _request.substr(checkIndex);
}

void Parser::parseRequestLine()
{
    assert(_lines.size() > 0);
    std::string requestLine = _lines[0];

    // first_ws指向请求行的第一个空白字符(' '或'\t')
    auto first_ws = std::find_if(requestLine.cbegin(), requestLine.cend(),
            [](char c)->bool { return (c == ' ' || c == '\t'); });

    // 如果请求行中没有出现空白则请求必定有错误
    if(first_ws == requestLine.cend())
    {
        //std::cout << "Request error." << std::endl;
        return;
    }
    // 截取请求方法
    _parseResult.method = std::string(requestLine.cbegin(), first_ws);

    // reverse_last_ws指向请求行中的最后一个空白字符(' '或'\t')
    auto reverse_last_ws = std::find_if(requestLine.crbegin(), requestLine.crend(),
            [](char c)->bool { return (c == ' ' || c == '\t'); });
    auto last_ws = reverse_last_ws.base();
    _parseResult.version = std::string(last_ws, requestLine.cend());

    while((*first_ws == ' ' || *first_ws == '\t') && first_ws != requestLine.cend())
        ++first_ws;

    --last_ws;
    while((*last_ws == ' ' || *last_ws == '\t') && last_ws != requestLine.cbegin())
        --last_ws;

    _parseResult.uri = std::string(first_ws, last_ws + 1);
}

void Parser::parseHeaders()
{
    assert(_lines.size() > 0);
    for(int i = 1; i < _lines.size(); ++i)
    {
        if(_lines[i].empty()) // 如果遇到空行说明请求解析完毕
        {
            return;
        }
        else if(strncasecmp(_lines[i].c_str(), "Host:", 5) == 0) // 处理"Host"头部字段
        {
            auto iter = _lines[i].cbegin() + 5;
            while(*iter == ' ' || *iter == '\t')
                ++iter;
            _parseResult.host = std::string(iter, _lines[i].cend());
        }
        else if(strncasecmp(_lines[i].c_str(), "Connection:", 11) == 0) // 处理"Connection"头部字段
        {
            auto iter = _lines[i].cbegin() + 11;
            while(*iter == ' ' || *iter == '\t')
                ++iter;
            _parseResult.connection = std::string(iter, _lines[i].cend());
        }
        else if (strncasecmp(_lines[i].c_str(), "Content-Type:", 13) == 0)
        {
            if (_lines[i].find("boundary=") != std::string::npos)
            {
                _hasSeparator = true;
                _separator = _lines[i].substr(_lines[i].find("boundary=") + 9);
                _separator = "--" + _separator;
            }
        }
        else
        {

        }
    }
}

void Parser::parsePost()
{

}

void Parser::parsePostWithSeparator()
{
    //std:: cout << "cut" << std::endl << _context << std::endl << "cut" << std::endl;
    int pos = 0;
    while ((pos = _context.find(_separator, pos)) != std::string::npos)
    {
        int start = pos + _separator.size();
        if (start >= _context.size() || (_context[start] == '-' && _context[start + 1] == '-'))
            break;
        int end = _context.find(_separator, start);
        pos = end;
        if (end == std::string::npos)
            end = _context.size();
        std::string current = _context.substr(start, end - start);
        //std::cout << "current " << current << std::endl;
        int nameStart = current.find("name=\"");
        if (nameStart == std::string::npos)
            break;
        std::string name = current.substr(
                    nameStart + 6,
                    current.find_first_of('\"', nameStart + 6) - nameStart - 6);
        std::string value;
        if (name == "file")
        {
            int filenameStart = current.find("filename=\"", nameStart);
            std::string filename = current.substr(
                        filenameStart + 10,
                        current.find_first_of('\"', filenameStart + 10) - filenameStart - 10);
            value = filename;
            //std::cout << "filename" << filename << std::endl;
            int fileStart = current.find("\r\n", filenameStart);
            fileStart = current.find("\r\n", fileStart + 2); //找两次是因为下面有一行描述类型的
            fileStart += 4;
            int fileEnd = current.size() - 2;
            if (fileEnd < fileStart)
                std::cout << "file error" << std::endl;
            else
            {
                //std::cout << "filev" << current.substr(fileStart, fileEnd - fileStart) << std::endl;
                std::ofstream tmpOne(_tmp + filename);
                tmpOne << current.substr(fileStart, fileEnd - fileStart);
                tmpOne.close();
            }

        }
        else
        {
            int valueStart = current.find_first_of('\"', nameStart + 6) + 5;
            value = current.substr(
                    valueStart,
                    current.find_first_of('r', valueStart) - valueStart);
        }
        _postParameter.push_back(name + "=" + value);
        //std::cout << "OO" << _postParameter.back() << std::endl;
    }
}
