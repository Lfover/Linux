#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void func(int signo)
{
}

int main()
{
	signal(2,func);
	while(1){
		printf("linux so easy");
		sleep(1);
	}
	return 0;
}
