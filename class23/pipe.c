
/*
    管道的读写特点
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
int main()
{

    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1)
    {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();
    if(pid > 0)
    {
        //父进程
        char buf[1024] = {0};
        int len = read(pipefd[0], buf, sizeof(buf));
        printf("parent recv %s,%d", buf, getpid());
    }
    else if(pid == 0)
    {
        //子进程
        char *str = "i am child";
        write(pipefd[1], str, strlen(str));
        
    }
    return 0;
}