#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/mman.h> /* for mmap */

int m = 4, x = 2, n = 3;
int** matrix1;
int** matrix2;
int** resultMatrix;

int p = 3;

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

void initResult()
{
	int i, j;
	resultMatrix = (int**) mmap(0, m * sizeof(int*), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);

	for(i = 0; i < x; i++)
	{
		resultMatrix[i] = (int*) mmap(0, n * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
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

int main(int argc, char* argv[])
{
	int i, j;
	int this = 0;
	pid_t pid;

	initMatrix1();
	fprintf(stderr, "\n");
	initMatrix2();
	fprintf(stderr, "\n");

	initResult();

	fprintf(stderr, "Process #%d says hello\n", this);

	for(i = 0; i < p; i++)
	{
		pid = fork();

		if(pid == 0)
		{
			this++;
			fprintf(stderr, "Process #%d says hello\n", this);
		}
		else
		{
			fprintf(stderr, "Process #%d spawned %d\n", this, pid);
			break;
		}
	}

	fprintf(stderr, "Process #%d will begin multiplying\n", this);

	for(j = 0; j < m; j++)
	{
		if(j % p == this)
		{
			fprintf(stderr, "Process #%d will begin line %d\n", this, j);
			resultMatrix[j] = multiplyAt(j);
			fprintf(stderr, "Process #%d multiplied line %d\n", this, j);
		}
	}

	fprintf(stderr, "Process #%d has finished multiplying\n", this);

	if(pid != 0)
	{
		fprintf(stderr, "Process #%d waiting for %d\n", this, pid);
		waitpid(pid, NULL, 0);
	}

	if(this != 0)
	{
		fprintf(stderr, "Process #%d says goodbye\n", this);
		exit(0);
	}

	fprintf(stderr, "Let's print that matrix!\n");

	printMatrix(resultMatrix, m, n);
}
