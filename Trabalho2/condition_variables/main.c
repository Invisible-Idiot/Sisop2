#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define RIGHT philosopher_number
#define LEFT philosopher_number-1
#define RANDOM (rand() % 10)+1

#define HUNGRY 1
#define EATING 2
#define THINKING 3

pthread_mutex_t* mutex;
pthread_cond_t * cond_available;
int* available;
pthread_mutex_t accessState;
pthread_cond_t cond_access;
int canAccess;
int* philosophers;
int n_mutex;

void printPhilosophers()
{
	int i;

	//pthread_mutex_lock(&accessState);
	//while(!canAccess)
		//pthread_cond_wait(&cond_access, &accessState);
	//canAccess = 0;

	for(i = 0; i < n_mutex; i++)
	{
		switch(philosophers[i])
		{
			case HUNGRY:
				fprintf(stderr, "H ");
				break;
			case EATING:
				fprintf(stderr, "E ");
				break;
			case THINKING:
				fprintf(stderr, "T ");
				break;
		}
	}

	fprintf(stderr, "\n");

	//canAccess = 1;
	//pthread_cond_signal(&cond_access);
	//pthread_mutex_unlock(&accessState);
}

void changeState(int philosopher_number, int state)
{
	//pthread_mutex_lock(&accessState);
	//while(!canAccess)
		//pthread_cond_wait(&cond_access, &accessState);
	//canAccess = 0;

	philosophers[philosopher_number] = state;

	//canAccess = 1;
	//pthread_cond_signal(&cond_access);
	//pthread_mutex_unlock(&accessState);
}

void philosopher(int philosopher_number)
{
	while(1)
	{
		changeState(philosopher_number, HUNGRY);

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

			changeState(philosopher_number, EATING);
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

			changeState(philosopher_number, EATING);
			sleep(RANDOM);

			available[LEFT] = 1;
			pthread_cond_signal(cond_available + LEFT);
			pthread_mutex_unlock(mutex + LEFT);

			available[RIGHT] = 1;
			pthread_cond_signal(cond_available + RIGHT);
			pthread_mutex_unlock(mutex + RIGHT);
		}
		changeState(philosopher_number, THINKING);
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
	pthread_t tid;
	int i;
	int philo_number;

	if(argc < 2)
	{
		printf("Please provide the number of philosophers\n");
		exit(0);
	}

	philo_number = atoi(argv[1]);

	if(philo_number <= 0)
	{
		printf("Number of philosophers must be an integer larger than 0\n");
		exit(0);
	}
	
	cond_available = (pthread_cond_t*) calloc(philo_number,sizeof(pthread_cond_t));
	available = (int*) calloc(philo_number,sizeof(int));
	mutex = (pthread_mutex_t*) calloc(philo_number, sizeof(pthread_mutex_t));
	pthread_mutex_init(&accessState, NULL);
	pthread_cond_init(&cond_access, NULL);
	canAccess = 1;
	philosophers = (int*) calloc(philo_number, sizeof(int));
	n_mutex = philo_number;

	for(i = 0; i < philo_number; i++)
	{
		pthread_cond_init(cond_available + i, NULL);
		available[i] = 1;
		pthread_mutex_init(mutex + i, NULL);
		philosophers[i] = HUNGRY;
	}

	for(i=0 ; i < philo_number ; i++)
		pthread_create(&tid,NULL,philosopher_thread,save(i));

	pthread_join(tid, NULL);
}
