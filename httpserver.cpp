#include "Server.h"
#include <string.h>

int main(int argc, char** argv)
{
    if(argc != 3 || (strcmp(argv[1], "-s") && strcmp(argv[1], "-c")))
    {
        std::cout << "Usage: " << std::endl
                  << "start Server" << argv[0] << "-s <filepath>" << std::endl;
                  //<< "command Server" << argv[0] << "-c <command>" << std::endl;
        return 0;
    }
    if (!strcmp(argv[1], "-s"))
    {
        Server server(argv[2]);
        server.start();
    }
    else
    {

    }
    return 0;
}
