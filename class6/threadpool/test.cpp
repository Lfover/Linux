#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>
//重命名函数指针
typedef void (*Handler)(int);

class QueueData
{
public:
	QueueData(int data, Handler Handler)
	{
		data_ = data;
		handler_ = Handler;
	}
	~QueueData()
	{

	}

	void Run()
	{
		handler_(data_);
	}


private:
	int data_;
	Handler handler_;
};

class ThreadPool
{
public:
	ThreadPool(int capacity, int thread_count)
	{
		capacity_ = capacity;
		thread_count_ = thread_count;
		pthread_mutex_init(&lock_, NULL);
		pthread_cond_init(&cons_cond_, NULL);
 	
		flag = 0;
	}

	~ThreadPool()
	{
		pthread_mutex_destroy(&lock_);
		pthread_cond_destroy(&cons_cond_);
	}

	int Oninit()
	{
		pthread_t tid;
		for(int i = 0; i < thread_count_; i ++)
		{
			int ret = pthread_create(&tid, NULL,PoolStart, (void*)this);
			if(ret < 0)
			{
				perror("pthread_create");
				return -1;
			}
		}
		return 0;
	}
	int Push(QueueData* qd)
	{
		pthread_mutex_lock(&lock_);
		if(flag)
		{
			pthread_mutex_unlock(&lock_);
			return -1;
		}

		que_.push(qd);
		pthread_mutex_unlock(&lock_);

		pthread_cond_signal(&cons_cond_);
		return 0;
	}

	void ThreadExit()
	{
		pthread_mutex_lock(&lock_);
		flag = 1;
		pthread_mutex_unlock(&lock_);
		pthread_cond_broadcast(&cons_cond_);
	}

private:
	void Pop(QueueData** qd)
	{
		*qd = que_.front();
		que_.pop();
	}
	
	static void* PoolStart(void* arg)
	{
		pthread_detach(pthread_self());
		ThreadPool* tp = (ThreadPool*)arg;
		while(1)
		{
			pthread_mutex_lock(&tp->lock_);
			while(tp->que_.empty())
			{
				if(tp->flag)
				{
					tp->thread_count_--;
					pthread_mutex_unlock(&tp->lock_);
					pthread_exit(NULL);
				}
				pthread_cond_wait(&tp->cons_cond_, &tp->lock_);
			}
			QueueData* qd;
			tp->Pop(&qd);
			pthread_mutex_unlock(&tp->lock_);

			qd->Run();
			delete qd;
		}
	}
private:
	std::queue<QueueData*> que_;
	size_t capacity_;
	
	pthread_mutex_t lock_;
	pthread_cond_t cons_cond_;

	int thread_count_;

	int flag;
};

void DealData(int data)
{
	printf("data:%d\n", data);
}



int main()
{
	ThreadPool* tp = new ThreadPool(4, 2);
	if(!tp)
	{
		printf("create ThreadPool failed\n");
		return -1;
	}
	int ret = tp->Oninit();
	if(ret < 0)
	{
		printf("create ThreadPool failed\n");
		return -1;
	}

	for(int i = 0; i < 100; i ++)
	{
		QueueData* qd = new QueueData(i, DealData);
		if(!qd)
		{
			continue;
		}
		tp->Push(qd);
	}

	sleep(10);
	tp->ThreadExit();
	delete tp;
	return 0;
}
