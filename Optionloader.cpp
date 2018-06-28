#include"Optionloader.h"


bool OptionLoader::LoadingOption()
{
    std::fstream optionFile(_path);
    if (!optionFile.is_open())
    {
        std::cout << "could not open the file" << std::endl;
        return 1;
    }
    std::string line;
    while (getline(optionFile, line))
    {
        auto key = line.substr(0, line.find(' '));
        auto value = line.substr(line.find(' ') + 1);
        if (addOption(key, value))
            return 1;
    }
    return 0;

}


bool OptionLoader::addOption(std::string key, std::string value)
{
    if (key == "host")
        _options.host = value;
    else if (key == "port")
        _options.port = atoi(value.c_str());
    else if (key == "root")
        _options.root = value;
    else if (key == "pthreadNum")
        _options.threadNum = atoi(value.c_str());
    else if (key == "fileType")
        _options.fileTypes.push_back(value);
    else
    {
        std::cout << "Wrong option, check the file" << std::endl;
        return 1;
    }
    return 0;
}
