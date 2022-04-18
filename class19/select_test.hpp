#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include "tcp_test.hpp"

class SelectSvr
{
public:
    SelectSvr()
    {
        FD_ZERO(&readfds_);
        nfds_ = -1;
    }
    ~SelectSvr()
    {
    }
    void AddFd(int fd)
    {
        FD_SET(fd, &readfds_);
        if(fd > nfds_)
        {
            nfds_ = fd;
        }
    }
    void DeleteFd(int fd)
    {
        FD_CLR(fd, &readfds_);
        for(int i = nfds_; i >= 0; i --)
        {
            if(FD_ISSET(i, &readfds_) == 0)
            {
                continue;
            }
            nfds_ = i;
            break;
        }
    }
    int SelectWait(struct timeval* tv, std::vector<TcpSvr>* vec)
    {
        fd_set tmp = readfds_;
       int ret =  select(nfds_ + 1, &tmp, NULL, NULL, tv);
       if(ret < 0)
       {
           perror("select");
           return -1;
       }
       else if(ret == 0)
       {
           printf("timeout\n");
           return 0;
       }
       //正常
       for(int i = 0; i <= nfds_; i ++)
       {
           if(FD_ISSET(i, &tmp) == 1)
           {
               TcpSvr ts;
               ts.SetSocketFd(i);
               vec->push_back(ts);
           }
       }
       return ret;
    }


private:
    fd_set readfds_;
    int nfds_;
};
