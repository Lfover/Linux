#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sockfd < 0)
    {
        perror("listen_sockfd");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1999);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int ret = bind(listen_sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }
    listen(listen_sockfd, 5);
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(listen_sockfd, &readfds);

    ret = select(listen_sockfd + 1, &readfds, NULL, NULL, NULL);
    if(ret < 0)
    {
        perror("select");
        return 0;
    }
    int new_sock = accept(listen_sockfd, NULL, NULL);
    if(new_sock < 0)
    {
        perror("accept");
        return 0;
    }
    printf("new_sock : %d\n", new_sock);

    return 0;
}
