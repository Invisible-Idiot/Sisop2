#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int** randMatrixGen(int lines, int columns)
{
    int max = 99,
        min = 0;
    int** matrix = (int**) calloc(lines, sizeof(int*));
    int x;
    for(x = 0; x < lines; x ++)
    {
        matrix[x] = (int*)calloc(columns, sizeof(int));
    }


    int i,j;
    for(i = 0; i < lines; i++)
        for(j = 0; j < columns; j++)
        {
            matrix[i][j] = (rand() % (max+1-min))+min;
        }
    return matrix;
}

int saveMatrix(int x, int y, int** matrix, int index)
{
      char buffer[10];
      sprintf (buffer, "in%d.txt", index);
      FILE *fp;
      int i;
      int j;

      fp = fopen(buffer, "w+");
      if (fp == NULL) {
         return 0;
      }
      fprintf(fp, "Linhas = %d\n", x);
      fprintf(fp, "Colunas = %d\n", y);
      for (i=0; i<x; ++i)
      {
          fprintf(fp, "%d", matrix[i][0]);

          for(j=1; j<y; ++j)
          {
              fprintf(fp, " %d", matrix[i][j]);
          }
          fprintf(fp, "\n");
      }

      fclose(fp);

	return 1;
}

int main(int argc, char* argv[])
{
    int lines, columns, qtd;
    printf("linhas: ");
    scanf("%d", &lines);
    printf("colunas: ");
    scanf("%d", &columns);
    printf("quantidade: ");
    scanf("%d", &qtd);
    int i;
    for(i = 0; i < qtd; i++)
    {
        int** matrix = randMatrixGen(lines, columns);
        saveMatrix(lines,columns, matrix, i);
    }
}
