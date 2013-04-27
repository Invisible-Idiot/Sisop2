#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define N 5

sem_t forks[N];

void think()
{
}

void eat()
{
}

void philosopher(int i)
{
	int left = i, right = (i + 1) % N;

	while(true)
	{
		think();
		sem_wait(forks + left);
		sem_wait(forks + right);
		eat();
		sem_post(forks + left);
		sem_post(forks + right);
	}
}

void philosopher_thread(void* arg)
{
	int* i = (int*) arg;

	philosopher(*i);
}

int main()
{
	int i;

	pthread_t philosophers[N];
	int indices[N];

	for(i = 0; i < N; i++)
	{
		sem_init(forks + i, 0, 1);
	}

	for(i = 0; i < N; i++)
	{
		indices[i] = i;
		pthread_create(philosophers + i, NULL, philosopher, indices + i);
	}
}
