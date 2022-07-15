#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int bowl = 0;
#define ThreadCount 4
//互斥锁
pthread_mutex_t g_mut;
//定义条件变量
pthread_cond_t cond;
pthread_cond_t prod;


//消费者线程的入口函数
void* Eat(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&g_mut);
        while(bowl <= 0)
        {
            pthread_cond_wait(&cond, &g_mut);
        }
        printf("i am %p, ,i eat %d\n", pthread_self(), bowl);
        bowl --;
        pthread_mutex_unlock(&g_mut);
        //通知生产者生产
        pthread_cond_signal(&prod);
    }
    return NULL;
}
//生产者线程的入口函数
void* Do(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&g_mut);
        while(bowl > 0)
        {
            pthread_cond_wait(&prod, &g_mut);
        }
        bowl ++;
        printf("i am %p, i do %d\n", pthread_self(), bowl);
        pthread_mutex_unlock(&g_mut);
        pthread_cond_signal(&cond);
    }
    
    return NULL;
}

int main()
{
    //动态初始化互斥锁
    pthread_mutex_init(&g_mut, NULL);
    //初始化条件变量
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&prod, NULL);

    pthread_t consume[ThreadCount], product[ThreadCount];
    for(int i = 0; i < ThreadCount; i ++)
    {
        int ret = pthread_create(&consume[i], NULL, Eat, NULL);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
        ret = pthread_create(&product[i], NULL, Do, NULL);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(int i = 0; i < ThreadCount; i ++)
    {
        pthread_join(consume[i], NULL);
        pthread_join(product[i], NULL);
    }
    //释放互斥锁
    pthread_mutex_destroy(&g_mut);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&prod);
    return 0;
}

