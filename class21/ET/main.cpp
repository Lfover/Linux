#include "tcp_test.hpp"
#include "epoll_test.hpp"

#define CHECK_RET(p) if(p < 0) {return -1;}


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("use: ./epoll_svr [ip] [port]\n");
        return 0;
    }
    string ip = argv[1];
    uint32_t port = atoi(argv[2]);

    TcpSvr ts;
    CHECK_RET(ts.CreateSocket());
    CHECK_RET(ts.Bind(ip, port));
    CHECK_RET(ts.Listen());

    EpollSvr es;
    CHECK_RET(es.CreateEpoll());
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = ts.GetSocketFd();

    CHECK_RET(es.EpollChange(EPOLL_CTL_ADD, ts.GetSocketFd(), &event));
    while(1)
    {
        struct epoll_event arr[10];
        int ret = es.EpollWait(arr, 10);
        if(ret < 0)
        {
            return -1;
        }
        else if(ret == 0)
        {
            continue;
        }
        for(int i = 0; i < ret; i ++)
        {
            if(arr[i].data.fd == ts.GetSocketFd())
            {
                TcpSvr client_info;
                struct sockaddr_in peer_addr;
                socklen_t peer_addrlen = sizeof(peer_addr);
                int ret = ts.Accept(&client_info, &peer_addr, &peer_addrlen);
                if(ret < 0)
                {
                    continue;
                }
                //添加新连接的套接字到eopll当中

                struct epoll_event event;
                event.events = EPOLLIN;
                event.data.fd = client_info.GetSocketFd();

                 CHECK_RET(es.EpollChange(EPOLL_CTL_ADD, client_info.GetSocketFd(), &event));
            }
            else
            {
                TcpSvr Ts;
                Ts.SetSocketFd(arr[i].data.fd);
                string data;
                int ret = Ts.Recv(&data);
                if(ret < 0)
                {
                    continue;
                }
                else if(ret == 0)
                {
                    es.EpollChange(EPOLL_CTL_DEL, Ts.GetSocketFd(), NULL);
                    close(Ts.GetSocketFd());
                    continue;
                }
                printf("client say: %s\n", data.c_str());
                data.clear();
                data += "i am server";
                Ts.Send(data);
            }
        }
    }
    return 0;
}
