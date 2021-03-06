/*
    #include <sys/types.h>
       #include <sys/wait.h>

       pid_t wait(int *status);
       等待任意一个子进程结束，如果任意一个子进程结束了，该函数就会回收子进程的资源
       参数
        -int *status
            进程退出时是状态信息，传入是一个int类型的地址，是一个传出参数
        返回值
            如果成功了，返回杯回收的子进程的id
            失败了，返回-1，所有子进程都结束，调用函数失败
            调用这个函数，父进程会被挂起，直到它的一个子进程退出或收到一个不能忽略的信号，才会被唤醒
            如果没有子进程了，立即返回-1
*/