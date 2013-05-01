#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define RIGHT philosopher_number
#define LEFT philosopher_number-1
#define RANDOM (rand() % 10)+1

sem_t * mutex;
int n_mutex;

void philosopher(int philosopher_number)
{
	while(1)
	{
		
		printf("filosfo %d tentando comer\n",philosopher_number);

		if(philosopher_number==0)
		{
			sem_wait(&(mutex[n_mutex-1]));
			sem_wait(&(mutex[RIGHT]));
			printf("filosofo %d comendo\n",philosopher_number);
			sleep(RANDOM);
			sem_post(&(mutex[n_mutex-1]));
			sem_post(&(mutex[RIGHT]));
		}
		else
		{
			sem_wait(&(mutex[RIGHT]));
			sem_wait(&(mutex[LEFT]));
			printf("filosofo %d comendo\n",philosopher_number);
			sleep(RANDOM);
			sem_post(&(mutex[RIGHT]));
			sem_post(&(mutex[LEFT]));
		}
		printf("filosofo %d pensando\n",philosopher_number);
		sleep(RANDOM);
	}

		
}

int main(int argc, char** argv)
{
	fprintf(stderr,"oba\n");
	pthread_t tid;
	int i;
	int philo_number = atoi(argv[1]);
	
	mutex = calloc(philo_number,sizeof(sem_t));
	n_mutex = philo_number;
		
	for(i=0 ; i < philo_number ; i++)
		sem_init(&mutex[i], 0, 1);

	for(i=0 ; i < philo_number ; i++)
		pthread_create(&tid,NULL,&philosopher,i);
	getchar();
}
