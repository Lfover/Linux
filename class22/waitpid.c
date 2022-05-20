/*
    #include <sys/types.h>
    #include <sys/wait.h>
     pid_t waitpid(pid_t pid, int *status, int options);
    回收指定进程号的子进程，可以设置是否阻塞
    pid
        pid > 0 某个子进程
        pid = 0 回收当前进程组的所有ID
        pid = -1 回收所有子进程
        pid < -1某个进程组id的绝对值
    -options
        设置阻塞或非阻塞
        0 阻塞
        WNOHANG 非阻塞
    返回值
        >0 返回子进程的id
        =0 options=WNOHANG 还有子进程活着
        -1 错误，没有子进程了
        
*/