#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    //创建监听套接字
    int fd = socket(PF_INET, SOCK_DGRAM, 0);

    if(fd == -1)
    {
        perror("socket");
        exit(-1);
    }
    //转换主机和网络地址,将绑定好的地址交给bind去绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    //绑定地址信息
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(-1);
    }
    //开始通信
    while(1)
    {
        //因为是被动连接，所以先接收数据
        char recvBuf[1024];
        char ipBuf[16];
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);

        int num = recvfrom(fd, recvBuf,sizeof(recvBuf), 0, (struct sockaddr *) &cliaddr, &len);
        printf("cli ip : %s, cli port %d\n",inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipBuf, sizeof(ipBuf)), ntohs(cliaddr.sin_port));
        printf("client say : %s\n", recvBuf);
        sendto(fd, recvBuf,strlen(recvBuf) + 1, 0, sizeof(struct sockaddr *)&cliaddr, sizeof(cliaddr));

    }
    close(fd);
    return 0;
}
