#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* Fun(void* arg)
{
    while(1)
    {
        printf("i am workthread\n");
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    //Fun是线程入口函数
    int ret = pthread_create(&tid, NULL, Fun, NULL);
    if(ret < 0)
    {
        perror("pthread_create");
        return -1;
    }
    while(1)
    {
        printf("i am mainthread\n");
        sleep(1);
    }

    return 0;
}
