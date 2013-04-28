#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../manager.h"
#include "../open.h"
#define SUCCESS 1
#define ARGUMENT_ERROR 2

int m, x, n;
int** matrix1;
int** matrix2;
int** resultMatrix;

int p;

void printMatrix(int** matrix, int lines, int columns)
{
	int i, j;

	for(i = 0; i < lines; i++)
	{
		for(j = 0; j < columns; j++)
		{
			fprintf(stderr, "%d\t", matrix[i][j]);
		}
		
		fprintf(stderr, "\n");
	}
}

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

void* fillMatrix(void* arg)
{
	int i;
	int* this = (int*) arg;

	for(i = 0; i < m; i++)
	{
		if(i % p == *this)
		{
			fprintf(stderr, "Process #%d multiplying line %d\n", *this, i);

			resultMatrix[i] = multiplyAt(i);
		}
	}
}

int** initResult()
{
	int i, j;
	int** matrix = (int**) calloc(m, sizeof(int*));

	for(i = 0; i < x; i++)
	{
		matrix[i] = (int*) calloc(n, sizeof(int));
	}

	return matrix;
}

int init()
{
	int x1, x2;

	matrix1 = open("../in1.txt", &m, &x1);
	matrix2 = open("../in2.txt", &x2, &n);

	if(x1 != x2)
		return ARGUMENT_ERROR;

	x = x1;

	resultMatrix = initResult();

	return SUCCESS;
}

int main(int argc, char* argv[])
{
	int i;
	int status;

	if(argc < 2)
	{
		printf("Please provide a number of processes\n");
		exit(0);
	}

	p = atoi(argv[1]);

	if(p == 0)
	{
		printf("The number of processes must be an integer larger than 0\n");
		exit(0);
	}

	status = init();

	if(status != SUCCESS)
	{
		printf("Matrices are incompatible\n");
		exit(0);
	}

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

	saveMatrix(m, n, resultMatrix);
}
