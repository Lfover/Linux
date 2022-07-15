#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int bowl = 0;
#define PthreadCount 2
//定义互斥锁，保证互斥
pthread_mutex_t lock_;
//定义条件变量保证同步
pthread_cond_t prod;
pthread_cond_t cons;

void* Eat(void* arg)
{
    //循环的吃
    while(1)
    {
        //首先加锁
        pthread_mutex_lock(&lock_);
        //如果现在没有面吃，等待
        while(bowl <= 0)
        {
            pthread_cond_wait(&cons, &lock_);
        }
        //有面，开始吃
        printf("i am %p, i eat %d\n", pthread_self(), bowl);
        //吃了一个面就减1
        bowl--;
        //离开这里把锁释放
        pthread_mutex_unlock(&lock_);
        //通知生产线程做面
        pthread_cond_signal(&prod);

    }
    return NULL;
}

void* Make(void* arg)
{
    //循环的做
    while(1)
    {
        //进来先加锁
        pthread_mutex_lock(&lock_);
        //还有面的时候就不着急做
        while(bowl > 0)
        {
            pthread_cond_wait(&prod, &lock_);
        }
        bowl++;
        printf("i am %p, i make %d\n", pthread_self(), bowl);
        //放锁
        pthread_mutex_unlock(&lock_);
        //通知来吃
        pthread_cond_signal(&cons);
    }
    return NULL;
}

int main()
{
    //互斥锁，条件变量的初始化
    pthread_mutex_init(&lock_, NULL);
    pthread_cond_init(&prod, NULL);
    pthread_cond_init(&cons, NULL);
    //创建线程
    pthread_t consume[PthreadCount], product[PthreadCount];
    for(int i = 0; i < PthreadCount; i ++)
    {
        int ret = pthread_create(&consume[i], NULL,Eat, NULL);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
        ret = pthread_create(&product[i], NULL,Make, NULL);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    //让系统自动回收资源，改变joinable属性
    for(int i = 0; i < PthreadCount; i ++)
    {
        pthread_join(consume[i], NULL);
        pthread_join(product[i], NULL);
    }
    //销毁互斥锁和条件变量
    pthread_mutex_destroy(&lock_);
    pthread_cond_destroy(&cons);
    pthread_cond_destroy(&prod);
    return 0;
}
