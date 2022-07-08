#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <semaphore.h>

//数组是否可以实现队列？
//1.读写下标的计算，pos=(pos + 1) % 数组的大小
//2.对读写数组实现线程安全的时候
//  互斥： sem_t lock; sem_init(&lock, 0, 1);
//  同步:
//          生产者信号量：sem_t prod ;sem_init(&prod, 0, 数组大小）
//          消费者信号量：sem_t cons ; sem_init(&cons, 0, 0)
#define CAPACITY 4  
#define THREADCOUNT 1
class RingQueue
{
    public:
    RingQueue()
        :vec_(CAPACITY)
    {
        capacity_ = CAPACITY;
        sem_init(&lock_, 0, 1);
        //目前数组当中，元素都是没有有效值的，所以信号在初始化的时候，可以紫云啊的数量救赎数组的大小
        sem_init(&prod_, 0, capacity_);
        //目前数组当中没有元素可以读，所以信号量当中的计数器未0
        sem_init(&cons_, 0, 0);
        pos_write_ = 0;
        pos_read_ = 0;
    }
    ~RingQueue()
    {
        sem_destroy(&lock_);
        sem_destroy(&prod_);
        sem_destroy(&cons_);
    }
    //写接口
    void Push(int data)
    {
        sem_wait(&prod_);
        sem_wait(&lock_);
        vec_[pos_write_] = data;
        pos_write_ = (pos_write_ + 1) % capacity_;
        sem_post(&lock_);
        sem_post(&cons_);
    }
    //读接口
    void Pop(int * data)
    {
        sem_wait(&cons_);
        sem_wait(&lock_);
        *data = vec_[pos_read_];
        pos_read_ = (pos_read_ + 1) % capacity_;
        sem_post(&lock_); 
        sem_post(&prod_);
    }
private:

//数组代替队列，只要满足先进先出的特性就可以
    std::vector<int> vec_;
    int capacity_;
    //保证互斥的信号量
    sem_t lock_;
    //保证同步的信号量
    sem_t prod_;
    sem_t cons_;

    //读写位置
    int pos_write_;
    int pos_read_;
};
//线程入口函数
void* ReadStart(void* arg)
{
    RingQueue* rq = (RingQueue*)arg;
    while(1)
    {
        int data;
        rq->Pop(&data);
        printf("i am %p, i consume %d\n", pthread_self(), data);
        data--;
    }
    return NULL;
}
void* WriteStart(void* arg)
{
    RingQueue* rq = (RingQueue*)arg;
    int data = 0;
    while(1)
    {
         rq->Push(data);
        printf("i am %p, i consume %d\n", pthread_self(), data);
        data++;
    }
   return NULL; 
}

int main()
{
    RingQueue* rq = new RingQueue();
    pthread_t cons[THREADCOUNT], prod[THREADCOUNT];
    for(int i = 0; i < THREADCOUNT; i ++)
    {
        int ret = pthread_create(&cons[i], NULL, ReadStart, (void*)rq);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
        ret = pthread_create(&prod[i], NULL, WriteStart, (void*)rq);
        if(ret < 0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(int i = 0; i < THREADCOUNT; i ++)
    {
        pthread_join(cons[i], NULL);
        pthread_join(prod[i], NULL);
    }
    delete rq;
    //rq = std::nullptr;
    return 0;
}
