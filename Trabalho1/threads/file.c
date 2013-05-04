#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "file.h"

#define BUFFER_SIZE 20
#define NEW_LINE '\n'
#define EQUAL '='
#define TERMINATOR_CHAR '\0'
#define SPACE ' '

int saveMatrix(int x, int y, int** matrix)
{
      FILE *fp;
      int i;
      int j;

      fp = fopen("out.txt", "w+");
      if (fp == NULL) {
         return 0;
      }
      fprintf(fp, "Linhas = %d\n", x);
      fprintf(fp, "Colunas = %d\n", x);
      for (i=0; i<x; ++i)
      {
          for(j=0; j<y; ++j)
          {
              fprintf(fp, "%d ", matrix[i][j]);
          }
          fprintf(fp, "\n");
      }

      fclose(fp);

	return 1;
}

int ** readMatrix(char* filename, int* nLine, int* nCol)
{
	
	
	char c;
	int i=0,j=0,str_iterator=0;
	char string[BUFFER_SIZE];
	FILE * newFile = fopen(filename, "r");
	
	
	if(newFile == NULL) return NULL;
	else
	{	
		for(c = getc(newFile) ; c != EQUAL ; c = getc(newFile)); // chega até o "LINHAS ="
		getc(newFile); // lê o espaço
		for(c = getc(newFile) ; c != NEW_LINE ; c = getc(newFile)) // lê o número
		{
			string[str_iterator] = c;
			str_iterator++;
		}
		string[str_iterator] = TERMINATOR_CHAR;

			
		*nLine = atoi(string);

		for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
			string[str_iterator] = 0;
		str_iterator =0 ; // reseta o iterador da string
		
		
		for(c = getc(newFile) ; c != EQUAL ; c = getc(newFile)); // chega até o "COLUNAS ="
		getc(newFile); // lê o espaço
		for(c = getc(newFile) ; c != NEW_LINE ; c = getc(newFile)) // lê o número
		{
			string[str_iterator] = c;
			str_iterator++;
		}
		string[str_iterator] = TERMINATOR_CHAR;
		*nCol = atoi(string);
		
		
		
		
		for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
			string[str_iterator] = 0;
		str_iterator =0 ; // reseta o iterador da string
		
		
		int **matrix = (int**)malloc((*nLine) * sizeof(int*));
		for(i=0 ; i< *nLine ; i++)
			matrix[i] = (int*) malloc((*nCol) * sizeof(int));
		
		i=0;j=0;
		c = getc(newFile);
		fpos_t pos;
		while(c!=EOF)
		{
			
			if(c==SPACE)
			{	
				string[str_iterator] = TERMINATOR_CHAR;
				matrix[i][j] = atoi(string);
				for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
					string[str_iterator] = 0;
				str_iterator =0 ; // reseta o iterador da string
										
				j++;
				
				
			} else 
			if(c==NEW_LINE)
			{
				fgetpos(newFile, &pos);
				c = getc(newFile);
				fsetpos(newFile, &pos);
				
				if(c!=EOF && c!=NEW_LINE)
				{
					string[str_iterator] = TERMINATOR_CHAR;
					matrix[i][j] = atoi(string);
					
					for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
						string[str_iterator] = 0;
					str_iterator =0 ; // reseta o iterador da string
					
					i++;
					j=0;
				}
			} else
			{
				string[str_iterator] = c;
				str_iterator++;
			}
			c = getc(newFile);
			
		}
		string[str_iterator] = TERMINATOR_CHAR;
		matrix[*nLine-1][*nCol-1] = atoi(string);
		
		fclose(newFile);
		
		return matrix;
		free(matrix);
	}
}
