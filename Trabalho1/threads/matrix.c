#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "file.h"
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

 void add(int i, int size, int j, int value, int* matrix)
 {
     matrix[(i * size) + j] += value;
 }

int** randMatrixGen(int lines, int columns)
{
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
            matrix[i][j] = rand() / (double)99;
        }
    return matrix;
}
