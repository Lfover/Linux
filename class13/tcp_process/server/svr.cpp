#include <stdio.h>
#include <unistd.h>
#include <string.h>

//网络编程必带头文件
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//进程需要带的头文件
#include <sys/wait.h>
#include <signal.h>
#include <iostream>

void sigcb(int signo)
{
	wait(NULL);
}
int main()
{
	//创建监听套接字
	int listen_sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if(listen_sock < 0)
	{
		perror("socket");
		return -1;
	}
	
	//绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(19990);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int ret = bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}
	//监听
	ret = listen(listen_sock, 5);
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}
	
	//监听成功后
	signal(SIGCHLD, sigcb);

	//连接时会创建一个新的套接字取连接
	while(1)
	{
		int new_sock = accept(listen_sock, NULL,NULL);
		//如果没有连接成功给就一直连接
		if(new_sock < 0)
		{
			perror("accept");
			continue;
		}
		//连接成功后，接收新的连接，创建子进程
		pid_t pid = fork();
		//子进程创建失败 
		if(pid < 0)
		{
			perror("fork");
			continue;
		}
		//子进程
		else if(pid == 0)
		{
			while(1)
			{
				//子进程负责连接
				close(listen_sock);
				char buf[1024] = {0};
				ssize_t recv_size = recv(new_sock, buf, sizeof(buf) - 1, 0);
				if(recv_size < 0)
				{
					perror("recv");
					continue;
				}
			  	else if(recv_size == 0)
				{
					printf("perr shutdown\n");
					close(new_sock);
					return 0;
				}
				//接收成功了
				printf("cli say:%s\n", buf);
				memset(buf, '\0',sizeof(buf));
				printf("please enter msg to cli:");
				fflush(stdout);
				std::cin >> buf;

				//发送数据
				ssize_t send_size = send(new_sock, buf, strlen(buf), 0);
				if(send_size < 0)
				{
					perror("send");
					continue;
				}
			}
		}
		//father
		//父进程负责建立监听，把建立连接的套接字关掉
		close(new_sock);
	}
	return 0;
}
					

	//创建新的套接字
	//建立连接
	//创建进程
	//发送数据
