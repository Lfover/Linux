#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        printf("这是父进程\n");
        printf("父进程id:%d, 子进程id %d\n", getppid(), getpid());
    }
    else if(pid == 0)
    {
        printf("这是子进程\n");
        printf("父进程id %d, 子进程id %d\n", getppid(), getpid());
    }
    for(int i = 0; i < 3; i ++)
    {
        printf("pid : %d\n", getpid());
        sleep(1);
    }
    return 0;
}