#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include "tcpserver.hpp"

class SelectSvr
{
    public:
    SelectSvr()
    {
        FD_ZERO(&readfds_);
        nfds_ = -1;
    }

    private:
    fd_set readfds_;
    int nfds_;
}