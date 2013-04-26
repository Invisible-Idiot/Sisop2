#include <stdlib.h>
#include <stdio.h>

void saveMatrix(int x, int** matrix)
{
      FILE *fp;
      int i;
      int j;

      fp = fopen("out.txt", "w+");
      if (fp == NULL) {
         printf("error");
         exit(0);
      }
      fprintf(fp, "Linhas = %d\n", x);
      fprintf(fp, "Colunas = %d\n", x);
      for (i=0; i<x; ++i)
      {
          for(j=0; j<x; ++j)
          {
              fprintf(fp, "%d ", matrix[i][j]);
          }
          fprintf(fp, "\n");
      }

      fclose(fp);
}

 int* initMatrix(int lines, int columns)
 {
     return (int*)malloc(sizeof(int)*lines*columns);
 }

 int get(int i, int size, int j, int* matrix)
 {
     return matrix[(i * size) + j];
 }

 void set(int i, int size, int j, int value, int* matrix)
 {
     matrix[(i * size) + j] = value;
 }
