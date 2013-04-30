#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define RIGHT philosopher_number
#define LEFT philosopher_number-1
#define RANDOM (rand() % 10)+1

pthread_mutex_t mutex;
pthread_cond_t * condition_var;
int n_mutex;

void philosopher(int philosopher_number)
{
	while(1)
	{
		
		printf("filosfo %d tentando comer\n",philosopher_number);
		sleep(RANDOM);
		if(philosopher_number==0)
		{
			pthread_cond_wait(&(condition_var[n_mutex-1]), &mutex);
			pthread_cond_wait(&(condition_var[RIGHT]), &mutex);
			printf("filosofo %d comendo\n",philosopher_number);
			pthread_cond_signal(&(condition_var[n_mutex-1]), &mutex);
			pthread_cond_signal(&(condition_var[RIGHT]), &mutex);
			
		}
		else
		{
			pthread_cond_wait(&(condition_var[LEFT]), &mutex);
			pthread_cond_wait(&(condition_var[RIGHT]), &mutex);
			printf("filosofo %d comendo\n",philosopher_number);
			pthread_cond_signal(&(condition_var[LEFT]), &mutex);
			pthread_cond_signal(&(condition_var[RIGHT]), &mutex);
		}
		printf("filosfo %d pensando\n",philosopher_number);
		sleep(RANDOM);
	}
		
}

int main(int argc, char** argv)
{
	fprintf(stderr,"oba\n");
	pthread_t tid;
	int i;
	int philo_number = atoi(argv[1]);
	
	condition_var = calloc(philo_number,sizeof(pthread_cond_t));
	n_mutex = philo_number;

	for(i=0 ; i < philo_number ; i++)
		pthread_create(&tid,NULL,&philosopher,i);
	getchar();
}
