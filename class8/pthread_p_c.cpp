#include <stdio.h>
#include <pthread.h>
#include <queue>

#define THREADCOUNT 1


int main()
{
	pthread_t cons[THREADCOUNT], prod[THREADCOUNT];
	
	for(int i = 0; i < THREADCOUNT; i ++)
	{
		int ret = pthread_create(&cons[i], NULL, ConsStart, NULL);
		if(ret < 0)
		{
			perror("pthread_create");
			return -1;
		}

		ret = pthread_create(&prod[i], NULL, ProdStart, NULL);
		if(ret < 0)
		{
			perror("pthread_create");
			return -1;
		}
