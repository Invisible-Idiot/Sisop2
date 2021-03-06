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

sem_t * mutex;
int* philosophers;
int n_mutex;
sem_t accessState;

void printPhilosophers()
{
	int i;

	sem_wait(&accessState);

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

	sem_post(&accessState);
}

void changeState(int philosopher_number, int state)
{
	sem_wait(&accessState);
	philosophers[philosopher_number] = state;
	sem_post(&accessState);
}

void philosopher(int philosopher_number)
{
	while(1)
	{
		changeState(philosopher_number, HUNGRY);
		printPhilosophers();

		if(philosopher_number==0)
		{
			sem_wait(&(mutex[n_mutex-1]));
			sem_wait(&(mutex[RIGHT]));
			changeState(philosopher_number, EATING);
			printPhilosophers();
			sleep(RANDOM);
			sem_post(&(mutex[n_mutex-1]));
			sem_post(&(mutex[RIGHT]));
		}
		else
		{
			sem_wait(&(mutex[RIGHT]));
			sem_wait(&(mutex[LEFT]));
			changeState(philosopher_number, EATING);
			printPhilosophers();
			sleep(RANDOM);
			sem_post(&(mutex[RIGHT]));
			sem_post(&(mutex[LEFT]));
		}
		changeState(philosopher_number, THINKING);
		printPhilosophers();
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
	
	mutex = (sem_t*) calloc(philo_number,sizeof(sem_t));
	philosophers = (int*) calloc(philo_number, sizeof(int));
	sem_init(&accessState, 0, 1);
	n_mutex = philo_number;
	
	for(i=0 ; i < philo_number ; i++)
		philosophers[i] = HUNGRY;
	
	for(i=0 ; i < philo_number ; i++)
		sem_init(&mutex[i], 0, 1);

	for(i=0 ; i < philo_number ; i++)
		pthread_create(&tid,NULL,philosopher_thread, save(i));

	pthread_join(tid, NULL);
}
