#include <stdlib.h>
#include <stdio.h>

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

int main(int argc, char* argv[])
{
	int i, j;
	int this = 0;
	pid_t pid;

	for(i = 0; i < p; i++)
	{
		pid = fork();

		if(pid = 0)
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
			resultMatrix[j] = multiplyAt(j);
		}
	}
}