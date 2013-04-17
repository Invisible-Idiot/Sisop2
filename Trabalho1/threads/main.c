#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int m, x, n;
int** matrix1;
int** matrix2;
int** resultMatrix;

int p;

int* multiplyAt(int lineNumber)
{
	int* line = (int*) calloc(n, sizeof(int));
	int i, j;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < x; j++)
		{
			line[i] += matrix1[lineNumber][j] * matrix2[j][i];
		}
	}

	return line;
}

void* fillMatrix(void* this)
{
	int i;

	for(i = 0; i < m; i++)
	{
		if(i % p == *this)
		{
			resultMatrix[i] = multiplyAt(i);
		}
	}
}

int main(int argc, char* argv[])
{
	int i;

	resultMatrix = (int**) calloc(m, sizeof(int*));

	pthread_t* pthreads = (pthread_t*) calloc(p, sizeof(pthread_t));
	int* threadNumber = (int*) calloc(p, sizeof(int));

	for(i = 0; i < p; i++)
	{
		threadNumber[i] = i;

		pthread_create(pthreads + i, NULL, fillMatrix, threadNumber + i);
	}

	for(i = 0; i < p; i++)
	{
		pthread_join(pthreads[i], NULL);
	}
}