#include <stdio.h>
#include <unistd.h>

//网络编程

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
int main()
{
	//创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}
	//服务端的资质信息结构，包含服务端的ip和port
	struct sockaddr_in svr_addr;
	
	svr_addr.sin_family = AF_INET;
	
	svr_addr.sin_addr.s_addr = inet_addr("192.168.83.128");
	svr_addr.sin_port = htons(19999);


	while(1)
	{
		std::string s;
		std::cin>>s;
		ssize_t send_size = sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr*)&svr_addr,sizeof(svr_addr));
		if(send_size < 0)
		{
			perror("sendto");
			continue;
		}


		char buf[1024] = { 0 };
		ssize_t rec_size = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, NULL, NULL);

		if(rec_size < 0)
		{
			perror("recvfrom");
			continue;
		}


		printf("svr say:%s\n", buf);

		close(sockfd);
	}

	return 0;
}
