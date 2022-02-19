

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <iostream>
#include <string.h>

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
	//建立连接
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

	int new_sock = -1;
	while(1)
	{
		new_sock = accept(listen_sock, NULL, NULL);
		if(new_sock < 0)
		{
			perror("accept");
			return -1;
		}
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
			close(listen_sock);
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
	close(new_sock);
	close(listen_sock);
	return 0;
}
	




