#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//select监控0号文件描述符的读事件
//select可以监控多个文件描述符吗
// 监控成功之后，事件集合当中是否去除了未就绪的摁键描述符
int main()
{
    //1.准备读事件集合，将0号文件描述符添加到事件集合当中去
    //2.监控
    //  2.1 阻塞
    //  2.2 非阻塞
    //  2.3 带有超时时间
    //3.判断是否监控成功
    //  3.1如果成功，读取内容
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sockfd < 0)
    {
        perror("listen_sockfd");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1999);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int ret = bind(listen_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }
    listen(listen_sockfd, 5);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    FD_SET(listen_sockfd, &readfds);
    while(1)
    {
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        fd_set tmp = readfds; 

        int ret = select(listen_sockfd + 1, &readfds, NULL, NULL, &tv);
        if(ret < 0)
        {      
            perror("select");
            return 0;
        }
        else if(ret == 0)
        {
            if(FD_ISSET(0, &tmp) == 1)
            {
                printf("0 in readfds\n");
            }
            else
            {
                printf("0 is not in readfds\n");
            }
            continue;
        }
        //需要判断是0号描述符就绪了，还是listen_sockfd就绪了

        if(FD_ISSET(0, &tmp) == 1)
        {
            char buf[1024] = {0};
            read(0, buf, sizeof(buf));
            printf("buf: %s\n", buf);
        }
        else if(FD_ISSET(listen_sockfd, &tmp) == 1)
        {
            //接收新连接
            int new_sock = accept(listen_sockfd, NULL, NULL);
            if(new_sock < 0)
            {
                perror("accept");
                continue;
            }
            printf("new_sock:%d\n", new_sock);
        }

    }
    return 0;
}
