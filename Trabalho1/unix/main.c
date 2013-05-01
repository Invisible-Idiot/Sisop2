#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/mman.h> /* for mmap */
#include "../manager.h"
#include "../open.h"
#define SUCCESS 1
#define ARGUMENT_ERROR 2
//#define MANUAL

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

#ifdef MANUAL
void initMatrix1()
{
	int i, j;
	matrix1 = (int**) calloc(m, sizeof(int*));

	for(i = 0; i < m; i++)
	{
		matrix1[i] = (int*) calloc(x, sizeof(int));
	}

	matrix1[0][0] = 7; matrix1[0][1] = 3;
	matrix1[1][0] = 2; matrix1[1][1] = 5;
	matrix1[2][0] = 6; matrix1[2][1] = 8;
	matrix1[3][0] = 9; matrix1[3][1] = 0;

	printMatrix(matrix1, m, x);
}

void initMatrix2()
{
	int i, j;
	matrix1 = (int**) calloc(x, sizeof(int*));

	for(i = 0; i < x; i++)
	{
		matrix1[i] = (int*) calloc(n, sizeof(int));
	}

	matrix1[0][0] = 7; matrix1[0][1] = 4; matrix1[0][2] = 9;
	matrix1[1][0] = 8; matrix1[1][1] = 1; matrix1[1][2] = 5;

	printMatrix(matrix1, x, n);
}
#endif

int* initResult()
{
	return (int*) mmap(0, m * n * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
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

/*
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
*/

void multiplyAt(int lineNumber)
{
	int i, j;
	int mul;

	
	for(i = 0; i < n; i++)
	{
		set(lineNumber, x, i, 0, resultMatrix);

		for(j = 0; j < x; j++)
		{
			mul = matrix1[lineNumber][j] * matrix2[j][i];
			add(lineNumber, x, i, mul, resultMatrix);
		}
	}
}

int main(int argc, char* argv[])
{
	int i, j;
	int this = 0;
	pid_t pid;
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

	//printMatrix(matrix1, m, x);
	//printMatrix(matrix2, x, n);

	//fprintf(stderr, "Process #%d says hello\n", this);

	for(i = 1; i < p; i++)
	{
		pid = fork();

		if(pid == 0)
		{
			this++;
			//fprintf(stderr, "Process #%d says hello\n", this);
		}
		else
		{
			//fprintf(stderr, "Process #%d spawned %d\n", this, pid);
			break;
		}
	}

	//fprintf(stderr, "Process #%d will begin multiplying\n", this);

	for(j = 0; j < m; j++)
	{
		if(j % p == this)
		{
			//fprintf(stderr, "Process #%d will begin line %d\n", this, j);

			fprintf(stderr, "Process #%d multiplying line %d\n", this, i);
			multiplyAt(j);

			//fprintf(stderr, "Process #%d multiplied line %d\n", this, j);
		}
	}

	//fprintf(stderr, "Process #%d has finished multiplying\n", this);

	if(pid != 0)
	{
		//fprintf(stderr, "Process #%d waiting for %d\n", this, pid);
		waitpid(pid, NULL, 0);
	}

	if(this != 0)
	{
		//fprintf(stderr, "Process #%d says goodbye\n", this);
		exit(0);
	}

	//fprintf(stderr, "Let's print that matrix!\n");

	//saveMatrix(m, n, resultMatrix);
}
