#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <iostream>

#define CAPACITY 1
#define THREADCOUNT 1


class RingQueue
{
public:
	RingQueue()
		:vec_(CAPACITY)
	{
		capacity_ = CAPACITY;
		sem_init(&lock_, 0, 1);
		sem_init(&prod_, 0, capacity_);
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

	void Push(int data)
	{
		sem_wait(&prod_);
		sem_wait(&lock_);
		vec_(pos_write_) = data;
		pos_write_ = (pos_write_ + 1) % capacity_;
		sem_post(&cons_);
	}

	void Pop(int* data)
	{
		sem_wait(&cons_);
		sem_wait(&lock_);
		*data = vec_[pos_read_];
		pos_read_ = (pos_read_ + 1)% capacity_;
		sem_post(&lock_);
		sem_post(&prod_);
	}




private:
	std::vector<int> vec_;
	int capacity_;
	sem_t lock_;
	sem_t prod_;
	sem_t cons_;

	int pos_write_;
	int pos_read_;
};

void* ReadStart(void* arg)
{
	RingQueue* rq = (RingQueue*)arg;
	while(1)
	{
		int data;
		rq->Pop(&data);
		printf("i am %p, i consume %d\n", pthread_self(), data);
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
		printf("i am %p, i product %d\n", pthread_self(), data);
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
	rq = NULL;
	return 0;
}
