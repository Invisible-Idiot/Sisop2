#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "file.h"

#define BUFFER_SIZE 20
#define NEW_LINE '\n'
#define EQUAL '='
#define TERMINATOR_CHAR '\0'
#define SPACE ' '

int saveMatrix(int x, int y, int* matrix)
{
      FILE *fp;
      int i;
      int j;

      fp = fopen("out.txt", "w+");
      if (fp == NULL) {
         return 0;
      }
      fprintf(fp, "Linhas = %d\n", x);
      fprintf(fp, "Colunas = %d\n", y);
      for (i=0; i<x; ++i)
      {
          for(j=0; j<y; ++j)
          {
              fprintf(fp, "%d ", get(i, y, j, matrix));
          }
          fprintf(fp, "\n");
      }

      fclose(fp);

	return 1;
}

int is_int(char c)
{
	if(
	c!='0'&&
	c!='1'&&
	c!='2'&&
	c!='3'&&
	c!='4'&&
	c!='5'&&
	c!='6'&&
	c!='7'&&
	c!='8'&&
	c!='9') 
		return 0;
	else
		return 1;
}

int ** readMatrix(char* filename, int* nLine, int* nCol)
{
	
	
	char c;
	int i=0,j=0,str_iterator=0;
	char string[BUFFER_SIZE];
	FILE * newFile = fopen(filename, "r");
	
	
	if(newFile == NULL) perror ("Error opening\n");
	else
	{	
		for(c = getc(newFile) ; c != EQUAL ; c = getc(newFile)); // chega atÃ© o "LINHAS ="
		getc(newFile); // lÃª o espaÃ§o
		for(c = getc(newFile) ; c != NEW_LINE ; c = getc(newFile)) // lÃª o nÃºmero
		{
			string[str_iterator] = c;
			str_iterator++;
		}
		string[str_iterator] = TERMINATOR_CHAR;

			
		*nLine = atoi(string);

		for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
			string[str_iterator] = 0;
		str_iterator =0 ; // reseta o iterador da string
		
		
		for(c = getc(newFile) ; c != EQUAL ; c = getc(newFile)); // chega atÃ© o "COLUNAS ="
		getc(newFile); // lÃª o espaÃ§o
		for(c = getc(newFile) ; c != NEW_LINE ; c = getc(newFile)) // lÃª o nÃºmero
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
		int clean_read=0;
		while(c!=EOF)
		{
			
			if(c==SPACE)
			{	
				string[str_iterator] = TERMINATOR_CHAR;
				if(str_iterator!=0)
				{
					matrix[i][j] = atoi(string);
					for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
						string[str_iterator] = 0;
					str_iterator =0 ; // reseta o iterador da string
										
					j++;
				}
				else
				{
					clean_read++;
					printf("Received excessive SPACE on line %d, ignoring...\n",i+1);
				}						
			} else 
			if(c==NEW_LINE)
			{	
				string[str_iterator] = TERMINATOR_CHAR;
				if(str_iterator!=0)
				{		
					matrix[i][j] = atoi(string);
					for(str_iterator =0 ; str_iterator < BUFFER_SIZE ; str_iterator++) // zera o buffer
						string[str_iterator] = 0;
					str_iterator =0 ; // reseta o iterador da string
			
					i++;
					j=0;
				}
				else
				{
					clean_read++;
					printf("Received excessive NEW_LINE on line %d, ignoring...\n",i+1);
				}
			} else
			if(is_int(c))
			{
				string[str_iterator] = c;
				str_iterator++;
			}
			else
			{
				clean_read++;
				printf("Received '%c', not an int, nor NEW_LINE, nor EOF on line %d, column %d\n",c,i+1,j+1);
			}
			c = getc(newFile);
			
		}
		if(str_iterator!=0)
		{
			string[str_iterator] = TERMINATOR_CHAR;
			printf("%c",string[0]);
			matrix[*nLine-1][*nCol-1] = atoi(string);
		}
		fclose(newFile);

		if(clean_read==0)
			return matrix;
		else
			return NULL;

		free(matrix);
	}
}
