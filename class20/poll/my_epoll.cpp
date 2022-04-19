#include <stdio.h>
#include <unistd.h>
#include <poll.h>

/*
 * 监控0号文件描述符的可读事件
 * 1.定义事件结构数组
 * 2.poll
 *  阻塞
 *  非阻塞
 *  带有超时时间
 *3.对0号文件描述符进行读操作
 */

int main()
{
    struct pollfd arr[5];
    arr[0].fd = 0;
    arr[0].events = POLLIN;
    /*
     * 返回值
     *  >0 表示就绪文件描述符的个数
     *  ==0 超时
     *  <0 表示监控出错
     */
    while(1)
    {
        int ret = poll(arr, 1, 1000);
        if(ret < 0)
        {
            perror("poll");
            return 0;
        }
        else if(ret == 0)
        {
            printf("timeout!\n");
            sleep(1);
            continue;
        }
        for(int i = 0; i < 5; i ++)
        {
            if(arr[i].revents == POLLIN)
            {
                char buf[1024] = {0};
                read(arr[i].fd, buf, sizeof(buf) - 1);
                printf("buf:%s", buf);
            }
        }
    }
    return 0;
}


