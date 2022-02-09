#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>

#define THREADCOUNT 2

class BlockQueue
{
	public:
		BlockQueue()
		{
			capacity_ = 10;
			pthread_mutex_init(&lock_, NULL);
			pthread_cond_init(&prod_cond_, NULL);
			pthread_cond_init(&cons_cond_, NULL);
		}
		~BlockQueue()
		{
			pthread_mutex_destroy(&lock_);
			pthread_cond_destroy(&cons_cond_);
			pthread_cond_destroy(&prod_cond_);
		}
void Push(int data)
{
	pthread_mutex_lock(&lock_);
	while(que_.size() >= capacity_)
	{
		pthread_cond_wait(&prod_cond_, &lock_);
	}

	que_.push(data);
	pthread_mutex_unlock(&lock_);
	pthread_cond_signal(&cons_cond_);
}

void Pop(int* data)
{
	pthread_mutex_lock(&lock_);
	while(que_.empty())
	{
		pthread_cond_wait(&cons_cond_, &lock_);
	}
	*data = que_.front();
	que_.pop();
	pthread_mutex_unlock(&lock_);
	pthread_cond_signal(&prod_cond_);
}
private:
	std::queue<int> que_;
	size_t capacity_;

	pthread_mutex_t lock_;
	pthread_cond_t prod_cond_;
	pthread_cond_t cons_cond_;
};

void* ConsumeStart(void* arg)
{
	BlockQueue* bq = (BlockQueue*)arg;
	while(1)
	{
		int data;
		bq->Pop(&data);
		printf("i am %p, i consume %d\n", pthread_self(), data);
		return NULL;
	}
}
void* ProductStart(void* arg)
{
        BlockQueue* bq = (BlockQueue*)arg;
        int data = 0;
	 while(1)
        {
                bq->Push(data);
                printf("i am %p, i product %d\n", pthread_self(), data);
                data++;
        }
	return NULL;
}
int main()
{
	pthread_t cons[THREADCOUNT], prod[THREADCOUNT];
	BlockQueue* bq = new BlockQueue();
	
	for(int i = 0; i < THREADCOUNT; i++)
	{
		int ret = pthread_create(&cons[i], NULL, ConsumeStart, (void*) bq);
		if(ret < 0)
		{
			perror("pthread_create");
			return -1;
		}

		ret = pthread_create(&prod[i], NULL, ProductStart, (void*) bq);
		if(ret < 0)
		{
			perror("pthread_create");
			return -1;
		}
	}
	for(int i = 0; i < THREADCOUNT; i++)
	{
		pthread_join(cons[i], NULL);
		pthread_join(prod[i], NULL);
	}
	return 0;
}



