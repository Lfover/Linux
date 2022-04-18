#include "tcp_test.hpp"
#include "select_test.hpp"

#define CHECK_RET(p) if(p < 0){return 0;}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("usage ./tcpsvr [ip]  [port]\n");
        return 0;
    }
    TcpSvr ts;
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    CHECK_RET(ts.CreateSocket());
    CHECK_RET(ts.Bind(ip, port));
    CHECK_RET(ts.Listen());
    
    SelectSvr ss;
    ss.AddFd(ts.GetSocketFd());
    while(1)
    {
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        std::vector<TcpSvr> vec;
        vec.clear();
        int ret = ss.SelectWait(&tv, &vec);
        if(ret < 0)
        {
            perror("select");
            return 0;
        }
        else if(ret == 0)
        {
            continue;
        }
        for(size_t i = 0; i < vec.size(); i ++)
        {
            if(vec[i].GetSocketFd() == ts.GetSocketFd())
            {
                //侦听套接字
                //新连接的套接字对应的TcpSvr
                TcpSvr new_ts;
                struct sockaddr_in peer_addr;
                socklen_t peer_addrlen = sizeof(peer_addr);
                ts.Accept(&new_ts, &peer_addr, &peer_addrlen);

                printf("new socket fd is %d, peer_ip:%s, peer_port:%d\n", new_ts.GetSocketFd(), inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));

                ss.AddFd(new_ts.GetSocketFd());
            }
            else
            {
                //新连接套接字
                TcpSvr new_tcp_svr = vec[i];
                string data;
                data.clear();
                int ret = new_tcp_svr.Recv(&data);
                if(ret < 0)
                {
                    perror("Recv");
                    close(new_tcp_svr.GetSocketFd());
                    ss.DeleteFd(new_tcp_svr.GetSocketFd());
                    continue;
                }
                else if(ret == 0)
                {
                    close(new_tcp_svr.GetSocketFd());
                    ss.DeleteFd(new_tcp_svr.GetSocketFd());
                    continue;
                }
                printf("Recv data %s\n", data.c_str());
                string send_data = "i am sever~~";
                new_tcp_svr.Send(send_data);
            }
        }
    }
    close(ts.GetSocketFd());

    return 0;
}

