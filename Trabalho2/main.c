#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 5

sem_t forks[N];

void think()
{
	sleep(rand() % 1);
}

void eat()
{
	sleep(rand() % 1);
}

void* antiPhilosopher(int i)
{
	int left = (i + 1) % N, right = i;

	while(1)
	{
		fprintf(stderr, "T");
		think();
		fprintf(stderr, "H");
		sem_wait(forks + left);
		sem_wait(forks + right);
		fprintf(stderr, "E");
		eat();
		sem_post(forks + left);
		sem_post(forks + right);
	}
}

void* philosopher(int i)
{
	int left = i, right = (i + 1) % N;

	while(1)
	{
		fprintf(stderr, "T");
		think();
		fprintf(stderr, "H");
		sem_wait(forks + left);
		sem_wait(forks + right);
		fprintf(stderr, "E");
		eat();
		sem_post(forks + left);
		sem_post(forks + right);
	}
}

void* antiPhilosopher_thread(void* arg)
{
	int* i = (int*) arg;

	return antiPhilosopher(*i);
}

void* philosopher_thread(void* arg)
{
	int* i = (int*) arg;

	return philosopher(*i);
}

int main()
{
	int i;

	pthread_t philosophers[N];
	int indices[N];

	for(i = 0; i < N; i++)
	{
		indices[i] = i;
		sem_init(forks + i, 0, 1);
	}

	pthread_create(philosophers, NULL, antiPhilosopher_thread, indices);

	for(i = 1; i < N; i++)
	{
		pthread_create(philosophers + i, NULL, philosopher_thread, indices + i);
	}

	for(i = 0; i < N; i++)
	{
		pthread_join(philosophers[i], NULL);
	}
}
