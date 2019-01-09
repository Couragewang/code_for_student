#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "comm.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage: " << proc << " server_ip server_port" << std::endl;
}
//client server_ip server_port
int main(int argc, char *argv[])
{
    if(argc != 3){
        Usage(argv[0]);
        exit(1);
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    bzero(&peer, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(atoi(argv[2]));
    peer.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr*)&peer, len) < 0){
        std::cerr << "connect error" << std::endl;
        exit(3);
    }

    char buf[1024];
    std::string message;
    Request_t rq;
    Response_t rsp;
    for(;;){
        memset(&rq, 0, sizeof(rq));
        memset(&rsp, 0, sizeof(rsp));
        std::cout << "Please Enter<x, y> ";
        std::cin >> rq.x >> rq.y;
        std::cout << "Please Enter Op<01234->+-*/%> ";
        std::cin >> rq.op;

        send(sock, &rq, sizeof(rq), 0);
        recv(sock, &rsp, sizeof(rsp), 0);
        std::cout << "status : " << rsp.status << std::endl;
        std::cout << "result : " << rsp.result << std::endl;
    }


    close(sock);
    return 0;
}


















