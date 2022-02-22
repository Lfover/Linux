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

#include "../connect.hpp"

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
	

	//当已完成连接队列为空的时候，调用accept函数会阻塞
	//连接时会创建一个新的套接字取连接
	int new_sock = -1;
	while(1)
	{
		new_sock = accept(listen_sock, NULL,NULL);
		//如果没有连接成功给就一直连接
		if(new_sock < 0)
		{
			perror("accept");
			return -1;
		}

		char buf[5] = {0};
		MyTCPHeader mth;
		ssize_t recv_size = recv(new_sock, &mth, sizeof(mth), 0);
		if(recv_size < 0)
		{
			perror("recv");
			continue;
		}
	  	else if(recv_size == 0)
		{
			printf("perr shutdown\n");
			close(new_sock);
			close(listen_sock);
			return 0;
		}
		//接收成功了
		printf("%d\n",mth.data_len_);
		//printf("cli say:%s\n", buf);
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
	return 0;
}
					

