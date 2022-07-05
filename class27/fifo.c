#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    int ret = mkfifo("./fifo_test", 0664);
    if(ret < 0)
    {
        perror("mkfifo");
        return -1;
    }
    printf("create success\n");
    return 0;
}
