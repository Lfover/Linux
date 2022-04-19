#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

class EpollSvr
{
public:
    EpollSvr()
    {
        epoll_fd_ = -1;
    }
    ~EpollSvr()
    {

    }
    int CreateEpoll()
    {
        epoll_fd_ = epoll_create(5);
        if(epoll_fd_ < 0)
        {
            perror("epoll_create");
            return -1;
        }
        return 0;
    }
    //add
    //del
    //modify
    //op:告知调用者
    //EPOLL_CTL_ADD
    //EPOLL_CTL_DEL
    //EPOLL_CTL_MOD
    int EpollChange(int op, int fd, struct epoll_event* ee)
    {
        int ret = epoll_ctl(epoll_fd_, op, fd, ee);
        if(ret < 0)
        {
            perror("epoll_ctl");
            return -1;
        }
        return 0;
    }
    int EpollWait(struct epoll_event* events, int maxcount, int timeout = -1)
    {
        int ret = epoll_wait(epoll_fd_, events, maxcount, timeout);
        if(ret < 0)
        {
            perror("epoll_wait");
            return -1;
        }
        else if(ret == 0)
        {
            printf("timeout...\n");
            return 0;
        }
    }
private:
    int epoll_fd_;

};
