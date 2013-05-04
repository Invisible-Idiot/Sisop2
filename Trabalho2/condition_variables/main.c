#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define RIGHT philosopher_number
#define LEFT philosopher_number-1
#define RANDOM (rand() % 10)+1

pthread_mutex_t* mutex;
pthread_cond_t * cond_available;
int* available;
int n_mutex;

void philosopher(int philosopher_number)
{
	while(1)
	{
		
		printf("filosfo %d tentando comer\n",philosopher_number);
		if(philosopher_number==0)
		{
			pthread_mutex_lock(mutex + RIGHT);
			while(!available[RIGHT])
				pthread_cond_wait(cond_available + RIGHT, mutex + RIGHT);
			available[RIGHT] = 0;
			pthread_mutex_lock(mutex + LEFT);
			while(!available[LEFT])
				pthread_cond_wait(cond_available + LEFT, mutex + LEFT);
			available[LEFT] = 0;
			printf("filosofo %d comendo\n", philosopher_number);
			sleep(RANDOM);
			available[RIGHT] = 1;
			pthread_cond_signal(cond_available + RIGHT);
			pthread_mutex_unlock(mutex + RIGHT);
			available[LEFT] = 1;
			pthread_cond_signal(cond_available + LEFT);
			pthread_mutex_unlock(mutex + LEFT);
		}
		else
		{
			pthread_mutex_lock(mutex + LEFT);
			while(!available[LEFT])
				pthread_cond_wait(cond_available + LEFT, mutex + LEFT);
			available[LEFT] = 0;
			pthread_mutex_lock(mutex + RIGHT);
			while(!available[RIGHT])
				pthread_cond_wait(cond_available + RIGHT, mutex + RIGHT);
			available[RIGHT] = 0;
			printf("filosofo %d comendo\n", philosopher_number);
			sleep(RANDOM);
			available[LEFT] = 1;
			pthread_cond_signal(cond_available + LEFT);
			pthread_mutex_unlock(mutex + LEFT);
			available[RIGHT] = 1;
			pthread_cond_signal(cond_available + RIGHT);
			pthread_mutex_unlock(mutex + RIGHT);
		}
		printf("filosfo %d pensando\n",philosopher_number);
		sleep(RANDOM);
	}
		
}

void* philosopher_thread(void* arg)
{
	int* philosopher_number = (int*) arg;

	philosopher(*philosopher_number);

	return NULL;
}

int* save(int i)
{
	int* p = (int*) malloc(sizeof(int));

	*p = i;

	return p;
}

int main(int argc, char** argv)
{
	fprintf(stderr,"oba\n");
	pthread_t tid;
	int i;
	int philo_number = atoi(argv[1]);
	
	cond_available = (pthread_cond_t*) calloc(philo_number,sizeof(pthread_cond_t));
	available = (int*) calloc(philo_number,sizeof(int));
	mutex = (pthread_mutex_t*) calloc(philo_number, sizeof(pthread_mutex_t));
	n_mutex = philo_number;

	for(i = 0; i < philo_number; i++)
	{
		pthread_cond_init(cond_available + i, NULL);
		available[i] = 1;
		pthread_mutex_init(mutex + i, NULL);
	}

	for(i=0 ; i < philo_number ; i++)
		pthread_create(&tid,NULL,philosopher_thread,save(i));

	pthread_join(tid, NULL);
}
