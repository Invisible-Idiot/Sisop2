#include <stdlib.h>
#include <string.h>
#include "message.h"

char* message(char* sender, char* content)
{
	if(strlen(sender) > USRNAMESIZE)
		return NULL;

	int totalSize = USRNAMESIZE + TEXTSIZE + 1 + 1;
	char delimiter[2] = "#";
	char* msg = (char*) malloc(totalSize);

	strncpy(msg, sender, USRNAMESIZE);
	strncat(msg, delimiter, 1);
	strncat(msg, content, TEXTSIZE);

	return msg;
}
message_t parseMessage(char* message)
{
	char delimiter[1]; delimiter[0] = '#';

	char* sender = strtok(message, delimiter);
	char* content = strtok(message, NULL);

	message_t msg; strcpy(msg.sender, sender); strcpy(msg.content, content);

	return msg;
}

size_t single(int x)
{
	int* p = (int*) malloc(sizeof(int));
	*p = x;

	return p;
}

void sendMessage(const char* myMessage, int mySocket)
{
	write(mySocket, single(strlen(myMessage)), sizeof(int));
	write(mySocket, myMessage, USRNAMESIZE + TEXTSIZE + 1 + 1);
}
