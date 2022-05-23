//向管道中写数据
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main()
{
    int ret = assert("fifo1", F_OK);
    if(ret == -1)
    {
        printf("管道不存在，创建管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }
    int fd = open("test", O_WRONLY);
    if(fd == -1) 
    {
        perror("open");
        exit(0);
    }
    for(int i = 0; i < 100; i ++)
    {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        printf("write data%s\n",buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }
    fclode(fd);
    return 0;
}
