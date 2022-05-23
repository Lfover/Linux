//从管道中读数据
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    int fd = open("test", O_RDONLY);
    if(fd == -1)
    {
        perrro("open");
        exit(0);
    }
    //读数据
    whiel(1)
    {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if(ret == 0)
        {
            printf("写端断开连接了\n");
            exit(0);
        }
        printf("recv buf %s\n", buf);
    }
    close(fd);
    return 0;
}