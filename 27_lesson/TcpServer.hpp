#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "ThreadPool.hpp"
#include "comm.hpp"

class Sock{
    private:
        int listen_sock;
        int port;
    public:
        Sock(const int &port_)
            :port(port_), listen_sock(-1)
        {
        }
        void Socket()
        {
            listen_sock = socket(AF_INET, SOCK_STREAM, 0);
            if(listen_sock < 0){
                std::cerr << "socket error!" << std::endl;
                exit(2);
            }
            int opt = 1;
            setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        }
        void Bind()
        {
            struct sockaddr_in local;
            bzero(&local, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(port);
 //           local.sin_addr.s_addr = inet_addr(ip.c_str());
            local.sin_addr.s_addr = htonl(INADDR_ANY);

            if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0){
                std::cerr << "bind error!" << std::endl;
                exit(3);
            }
        }
        void Listen()
        {
            if(listen(listen_sock, 5) < 0){
                std::cerr << "listen error" << std::endl;
                exit(4);
            }
        }
        int Accept()
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if(sock < 0){
                std::cerr << "accept error" << std::endl;
                return -1;
            }

            return sock;
        }
        ~Sock()
        {
            close(listen_sock);
        }
};

class Server;

typedef struct {
    Server *sp;
    int sock;
}data_t;

class Server{
    private:
        Sock sock;
        ThreadPool pool;
    public:
        Server(const int &port_):sock(port_), pool(5)
        {
        }
        void InitServer()
        {
            signal(SIGCHLD, SIG_IGN);
            sock.Socket();
            sock.Bind();
            sock.Listen();
            pool.InitThreadPool();
        }
        static void Service(int sock)
        {
            Request_t rq;
            Response_t rsp;
            recv(sock, &rq, sizeof(rq), 0);
            rsp.status = 0;
            switch(rq.op){
                case 0:
                    rsp.result = rq.x + rq.y;
                    break;
                case 1:
                    rsp.result = rq.x - rq.y;
                    break;
                case 2:
                    rsp.result = rq.x * rq.y;
                    break;
                case 3:
                    {
                        if(rq.y == 0){
                            rsp.status = 1;
                        }else{
                            rsp.result = rq.x / rq.y;
                        }
                    }
                    break;
                case 4:
                    break;
                default:
                    rsp.status = 2;
                    break;
            }
            send(sock, &rsp, sizeof(rsp), 0);

           // std::unordered_map<std::string, std::string> dict;
           // dict.insert(std::make_pair("hello", "你好"));
           // dict.insert(std::make_pair("bit", "比特"));
           // dict.insert(std::make_pair("xust", "西安科技大学"));
           // dict.insert(std::make_pair("xpu", "西安工程大学"));
           // dict.insert(std::make_pair("apple", "苹果"));
           // char buf[1024];
           // for( ; ; ){
           //     ssize_t s = read(sock, buf, sizeof(buf)-1);
           //     if(s > 0){
           //         buf[s] = 0;
           //         std::cout << buf << std::endl;
           //         std::string key = buf;
           //         std::string val = dict[key];
           //         if(val.empty()){
           //             val="null";
           //         }
           //         write(sock, val.c_str(), val.size());
           //     }
           //     else if(s == 0){
           //         std::cout << "client is quit..." << std::endl;
           //         break;
           //     }
           //     else{
           //         std::cerr << "read error!" << std::endl;
           //         break;
           //     }
           // }
            close(sock);
        }
 //       static void *ThreadRun(void *arg)
 //       {
 //           pthread_detach(pthread_self());
 //           data_t *d = (data_t*)arg;
 //           Server *sp = d->sp;
 //           int sock = d->sock;
 //           delete d;
 //           sp->Service(sock);
 //       }
        void Run()
        {
            for( ; ; ){
                int new_sock = sock.Accept();
                if(new_sock < 0){
                    continue;
                }
                std::cout << "Get A New Client..." << std::endl;
                Task t(new_sock, Server::Service);
                pool.AddTask(t);
              //  data_t *d = new data_t;
              //  d->sp = this;
              //  d->sock = new_sock;
              //  pthread_t tid;
              //  pthread_create(&tid, NULL, ThreadRun, (void *)d);
            //    pid_t id = fork();
            //    if(id == 0){//child
            //        Service(new_sock);
            //        exit(0);
            //    }
            //    close(new_sock);
            }
        }
        ~Server()
        {
        }
};
















