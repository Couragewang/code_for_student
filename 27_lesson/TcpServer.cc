#include "TcpServer.hpp"

void Usage(std::string proc_)
{
    std::cout << "Usage: " << proc_ << " local_port" << std::endl;
}

//server port
int main(int argc, char *argv[])
{
    if(argc != 2){
        Usage(argv[0]);
        exit(1);
    }
    int port = atoi(argv[1]);

    Server *sp = new Server(port);
    sp->InitServer();
    sp->Run();
    delete sp;
    return 0;
}
