/*
创建fifo 
    #include <sys/types.h>
    #include <sys/stat.h>

    int mkfifo(const char *pathname, mode_t mode);
        参数
            -pathname:管道名称的路径
            -mode :文件的权限
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    //判断文件是否存在
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

    return 0;
}