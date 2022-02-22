#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include "../connect.hpp"

int main()
{
	//创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	//调用连接接口
	struct sockaddr_in peeraddr;
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(19990);
	//inet_addr finish two things
	peeraddr.sin_addr.s_addr = inet_addr("192.168.83.128");
	
	int ret = connect(sockfd,(struct sockaddr*)&peeraddr, sizeof(peeraddr)); 
	if(ret < 0)
	{
		perror("connect");
		return -1;
	}
	int new_sock = -1;
	new_sock = accept(new_sock, NULL,NULL);
	if(new_sock < 0)
	{
		perror("accept");
		return -1;
	}
	
	

	while(1)
	{
		//发送
		char buf[1024] = {0};
		printf("please enter msg to svr: ");
		fflush(stdout);
		std :: cin >> buf;
		//定义一个结构体，报头
		MyTCPHeader mth;
		//data_len_的大小是那会数据以后才知道
		mth.data_len_ = strlen(buf);
 
		//再调用一次send函数，将报头也发送过去
		ssize_t send_size = send(sockfd, &mth, sizeof(mth), 0);
		if(send_size < 0)
		{
			perror("send");
			continue;
		}
		send_size = send(sockfd, buf, strlen(buf), 0);
		//接收
		if(send_size < 0)
		{
			perror("send");
			continue;
		}
		//先清空
		memset(buf, '\0', sizeof(buf));
		ssize_t recv_size = recv(sockfd, buf, sizeof(buf) - 1, 0);
		if(recv_size < 0)
		{
			perror("recv");
			continue;
		}
		else if(recv_size == 0)
		{
			printf("peer shutdown connect\n");
			close(sockfd);
			return 0;
		}
		printf("svr say:%s\n", buf);
	}
	close(sockfd);
	return 0;
}
