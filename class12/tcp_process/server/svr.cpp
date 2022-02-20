

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>


void sigcb(int signo)
{
	wait(NULL);
}


int main()
{
	//listen_sock是用来和客户端建立连接的
	//监听套接字
	int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listen_sock < 0)
	{
		perror("socket");
		return -1;
	}
	//绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(19998);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int ret = bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}

	//连接队列
	ret = listen(listen_sock, 5);
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}
	//new_sock用来和客户端通信的	
	//当已完成连接队列为空的时候，调用accept函数会阻塞
	signal(SIGCHLD, sigcb);
	while(1)
	{
		int new_sock = accept(listen_sock, NULL, NULL);
		if(new_sock < 0)
		{
			perror("accept");
			continue;
		}

		//接收了新的连接了
		pid_t pid = fork();
		if(pid < 0)
		{
			perror("fock");
			continue;
		}
		else if(pid == 0)
		{
			//child
			while(1)
			{
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

				printf("cli say: %s\n", buf);
				memset(buf, '\0', sizeof(buf));
				printf("please enter msg to cli: ");
				fflush(stdout);
				std::cin >> buf;
				
				ssize_t send_size = send(new_sock, buf, strlen(buf), 0);
				if(send_size < 0)
				{
					perror("send");
					continue;
				}
			}
		}
			//father 
			close(new_sock);
		}
	return 0;
}
	




