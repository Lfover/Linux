#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void* myStartThread(void* arg)
{
	int* i = (int *)arg;
	while(1)
	{
		printf("i am workthread%p, i = %d\n", pthread_self(), *i);
		sleep(1);
	}
}


int main()
{
	pthread_t tid;
	for(int i = 0; i < 4; i ++){
	int ret = pthread_create(&tid, NULL, myStartThread, (void *)&i);
	if(ret < 0)
	{
		perror("pthread_create");
		return -1;
	}
	}
	
	while(1)
	{
		printf("i am main thread\n");
		sleep(1);
	}
	return 0;
}
