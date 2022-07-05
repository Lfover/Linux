#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return -1;
    }
    else if(pid == 0)
    {
        //子进程
        sleep(5);
        exit(0);
    }
    else
    {
        while(1)
        {
            sleep(1);
        }
    }

    return 0;
}
