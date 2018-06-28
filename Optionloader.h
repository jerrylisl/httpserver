#ifndef OPTIONLOADER
#define OPTIONLOADER

#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include"Base.h"


struct Options
{
    std::string host = "localhost";
    int port = 9001;
    std::string root = "/home/jerry/page/";
    int threadNum = 1;
    std::vector<std::string> fileTypes;
};

class OptionLoader : Noncopyable
{
public:
    explicit OptionLoader(std::string path)
        :_path(path) {}
    bool LoadingOption();

    std::string getHost()
    {
        return _options.host;
    }

    int getPort()
    {
        return _options.port;
    }

    std::string getRoot()
    {
        return _options.root;
    }

    int getThreadNum()
    {
        return _options.threadNum;
    }

    std::vector<std::string> getFileTypes()
    {
        return _options.fileTypes;
    }




private:
    bool addOption(std::string key, std::string value);

    std::string _path;
    Options _options;

};


#endif // OPTIONLOADER

