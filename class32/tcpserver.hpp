#pragma once
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <netinet/in.h>

using namespace std;

//定义一个TCP通信的接口类
class TcpSvr
{
    public:
    //创建套接字，bind,listen,accept,接收数据，发送数据
    TcpSvr()
    {
        socket_fd_ = -1;
    }
    ~TcpSvr()
    {}
    //创建套接字
    int CreateSocket()
    {
        socket_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(socket_fd_ < 0)
        {
            perror("socket");
            return -1;
        }
        return 0;
    }
    int Bind(const string& ip, const uint16_t port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        int ret = bind(socket_fd_, (struct sockaddr*)&addr, sizeof(addr));
        if(ret < 0)
        {
            perror("bind");
            return -1;
        }
        return 0;
    }
    int Listen(int backlog = 5)
    {
        int ret = listen(socket_fd_, backlog);
        if(ret < 0)
        {
            perror("listen");
            return -1;
        }
        return 0;
    }
    int Accept(TcpSvr* ts, struct sockaddr_in* peer_addr, socklen_t* peer_addrlen)
    {
        int new_sock = accept(socket_fd_, (struct sockaddr*)peer_addr, peer_addrlen);
        if(new_sock < 0)
        {
            perror("accept");
            return -1;
        }
        ts->SetSocketFd(new_sock);
        return 0;
    }
    void SetSocketFd(int fd)
    {
        socket_fd_ = fd;
    }
    int Send(const string& data)
    {
        ssize_t send_size = send(socket_fd_, data.c_str(),data.size(), 0);
        if(send_size < 0)
        {
            perror("send");
            return -1;
        }
        return send_size;
    }
    int recv(string* data)
    {
        char buf[1024] = {0};
        ssize_t recv_size = recv(socket_fd_, buf, sizeof(buf) - 1, 0);
        if(recv_size < 0)
        {
            perror("recv");  
            return -1;
        }
        else if(recv_size == 0)
        {
            printf("peer shutdown...\n");
            return 0;
        }
        data->assign(buf, strlen(buf));
        return recv_size;
    }
    int GetSockFd()
    {
        return socket_fd_;
    }
    private:
    int socket_fd_;
};