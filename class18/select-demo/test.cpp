#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

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
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    while(1)
    {
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        fd_set tmp = readfds; 

        int ret = select(1, &readfds, NULL, NULL, &tv);
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
        char buf[1024] = {0};
        read(0, buf, sizeof(buf));
        printf("buf: %s\n", buf);
    }
    return 0;
}
