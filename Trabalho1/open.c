#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 20
#define NEW_LINE '\n'
#define EQUAL '='
#define TERMINATOR_CHAR '\0'
#define SPACE ' '

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

int ** open(char* filename, int* nLine, int* nCol)
{
	
	
	char c;
	int i=0,j=0,str_iterator=0;
	char string[BUFFER_SIZE];
	FILE * newFile = fopen(filename, "r");
	
	
	if(newFile == NULL) perror ("Error opening\n");
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
			//printf("%c",string[0]);
			matrix[*nLine-1][*nCol-1] = atoi(string);
		}
		if(clean_read==0)
			printf("Read OK!\n");
		else
			printf("%d errors...\n",clean_read);
		fclose(newFile);
		
		return matrix;
		free(matrix);
	}
}


int main()
{
	int i=0,j=0;
	int l,c;
	int ** m = open("in1.txt",&l,&c);
	printf("L = %d, C = %d\n\n",l,c);
	for(i=0 ; i < l ; i++)
	{
		for(j=0; j<c ; j++)
			printf("%d ",m[i][j]);
		printf("\n");
	}
	free(m);
}
