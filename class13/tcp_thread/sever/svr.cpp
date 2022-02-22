#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>

struct SockVal
{
	int new_sock_;
};

void* TcpThread(void* arg)
{
	pthread_detach(pthread_self());
	SockVal* sv = (struct SockVal*)arg;
	//开始接收数据
	while(1)
	{
		char buf[1024] = {0};
		ssize_t recv_size = recv(sv->new_sock_, buf, sizeof(buf) - 1, 0);
		if(recv_size < 0)
		{
			perror("recv");
			continue;
		}
		else if(recv_size == 0)
		{
			printf("perr shutdown\n");
			close(sv->new_sock_);
			delete sv;
			return NULL;
		}
		//接收成功		
		printf("cli say: %s\n", buf);
		memset(buf, '\0', sizeof(buf));
		printf("please enter msg to cli： ");
		fflush(stdout);
		std::cin >> buf;
		//发送数据
		ssize_t send_size = send(sv->new_sock_, buf, strlen(buf), 0);
		if(send_size < 0)
		{
			perror("send");
			continue;
		}
	}
	return NULL;
}

int main()
{
	//创建监听套接字
	int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//监听套接字创建失败	
	if(listen_sock < 0)
	{
		perror("socket");
		return -1;
	}
	//监听套接字创建成功
	//之后需要绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(19998);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int ret = bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr));
	//绑定失败了
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}
	//绑定成功了
	//调用listen接口，接收返回值
	ret = listen(listen_sock, 5);	
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}
	while(1)
	{
		int new_sock = accept(listen_sock, NULL,NULL);
		if(new_sock < 0)
		{
			perror("accept");
			continue;
		}
		//创建多线程
		pthread_t tid;
		SockVal* sv = new SockVal();
		sv->new_sock_ = new_sock;

		//接收线程创建接口的返回值
		int ret = pthread_create(&tid, NULL, TcpThread,(void*)sv);
		if(ret < 0)
		{
			perror("pthread_create");
			continue;
		}
	}
	return 0;
}
