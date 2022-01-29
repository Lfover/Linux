#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#define KEY 0x88888888
int main()
{
	int shmid = shmget(KEY, 1024, IPC_CREAT | 0664);
	if(shmid < 0)
	{
		perror("shmget");
		return -1;
	}
	//0表示可读可写
	void * lp = shmat(shmid, NULL, 0);
	printf("%p\n", lp);
	strcpy((char*)lp,"Linux-hahha");
	printf("%s\n",(char*)lp);
	while(1)
	{
		sleep(1);
	}
	return 0;
}
