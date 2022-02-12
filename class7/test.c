#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	pid_t pid;
	printf("Before:pid id %d\n", getpid());
	if((pid = fork()) == -1)
	{
		//进程创建失败
		perror("fork()");
		exit(1);
	}
	//进程创建成功
	printf("after:pid is %d, fork return %d\n", getpid(), pid);
	sleep(1);
	return 0;
}
