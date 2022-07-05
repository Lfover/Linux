#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#define K 0x89898989
int main()
{

    int ret = shmget(K, 1024, IPC_CREAT);
    if(ret < 0)
    {
        perror("shmget");
        return -1;
    }
    //创建成功了共享内存
    while(1)
    {
        sleep(1);
    }

    return 0;
}
