#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/mman.h> /* for mmap */
#include "matrix.h"
#include "file.h"
#define SUCCESS 0
#define MATRIX1_ERROR 1
#define MATRIX2_ERROR 2
#define INCOMPATIBLE_ERROR 3

int m, x, n;
int** matrix1;
int** matrix2;
int* resultMatrix;

int p;

void printMatrix(int** matrix, int lines, int columns)
{
	int i, j;

	for(i = 0; i < lines; i++)
	{
		for(j = 0; j < columns; j++)
		{
			printf("%d\t", matrix[i][j]);
		}

		printf("\n");
	}
}

void printResult(int* matrix, int lines, int columns)
{
	int i, j;

	for(i = 0; i < lines; i++)
	{
		for(j = 0; j < columns; j++)
		{
			printf("%d\t", get(i, columns, j, matrix));
		}

		printf("\n");
	}
}

int* initResult(int lines, int columns)
{
	return (int*) mmap(0, lines * columns * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
}

int init()
{
	int x1, x2;

	matrix1 = readMatrix("in1.txt", &m, &x1);

	if(matrix1 == NULL)
		return MATRIX1_ERROR;

	matrix2 = readMatrix("in2.txt", &x2, &n);

	if(matrix2 == NULL)
		return MATRIX2_ERROR;

	if(x1 != x2)
		return INCOMPATIBLE_ERROR;

	x = x1;

	resultMatrix = initResult(m, n);

	return SUCCESS;
}

void multiplyAt(int lineNumber)
{
	int i, j;
	int mul;

	for(i = 0; i < n; i++)
	{
		set(lineNumber, n, i, 0, resultMatrix);

		for(j = 0; j < x; j++)
		{
			mul = matrix1[lineNumber][j] * matrix2[j][i];
			add(lineNumber, n, i, mul, resultMatrix);
		}
	}
}

int main(int argc, char* argv[])
{
	int i, j;
	int this = 0;
	pid_t pid;
	int status;
	int success;

	if(argc < 2)
	{
		printf("Please provide a number of processes\n");
		exit(0);
	}

	p = atoi(argv[1]);

	if(p <= 0)
	{
		printf("The number of processes must be an integer larger than 0\n");
		exit(0);
	}

	status = init();

	switch(status)
	{
		case MATRIX1_ERROR:
			printf("Couldn't read in1.txt\n");
			exit(0);
		case MATRIX2_ERROR:
			printf("Couldn't read in2.txt\n");
			exit(0);
		case INCOMPATIBLE_ERROR:
			printf("Matrices are incompatible\n");
			exit(0);
		default: /* case SUCCESS: */
			break;
	}

	//printMatrix(matrix1, m, x);
	//printMatrix(matrix2, x, n);

	for(i = 1; i < p; i++)
	{
		pid = fork();

		if(pid == 0)
		{
			this++;
		}
		else
		{
			break;
		}
	}

	for(j = 0; j < m; j++)
	{
		if(j % p == this)
		{
			//printf("Process #%d multiplying line %d\n", this, j);
			multiplyAt(j);
			//sleep(1);
		}
	}

	if(pid != 0)
	{
		waitpid(pid, NULL, 0);
	}

	if(this != 0)
	{
		pthread_exit(NULL);
	}

	success = saveMatrix(m, n, resultMatrix);

	if(!success)
	{
		printf("Couldn't write to file out.txt");
	}

	for(i = 0; i < m; i++)
	{
		free(matrix1[i]);
	}

	free(matrix1);

	for(i = 0; i < x; i++)
	{
		free(matrix2[i]);
	}

	free(matrix2);

	//free(resultMatrix);

	return 0;
}
