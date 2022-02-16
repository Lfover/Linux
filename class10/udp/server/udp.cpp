#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <sting>

int main()
{
	// 创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	//绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("0. 0. 0. 0");
	addr.sin_port = htons(1000);

	int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
 	}

	while(1)
	{
		char buf[1024] = {0};
		struct sockaddr_in peer_addr;
		socklen_t peer_addr_len = sizeof(peer_addr);
		ssize_t recv_size = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&peer_addr, &peer_addr_len);
		if(recv_size < 0)
		{
			perror("recvfrom");
			continue;
		}
		printf("cli say:%s\n", buf);


		std::string s;
		std::cin >> s;
		ssize_t send_size = sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr*)&peer_addr, peer_addr_len);
		if(send_size < 0)
		{
			perror("sendto");
			continue;
		}
	}

	close(sockfd);
	return 0;
}
