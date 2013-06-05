#include <stdlib.h>
#include <string.h>
#include "message.h"

char* message(const char* sender, const char* content)
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
	size_t* messageLength = single(strlen(myMessage));

	write(mySocket, messageLength + 1, sizeof(size_t));
	write(mySocket, myMessage, messageLength + 1);

	free(messageLength);
}

size_t receiveLength(int mySocket)
{
	size_t* length_p = (size_t*) malloc(sizeof(size_t));
	char buffer[sizeof(size_t)];
	int bytesRead = 0;

	while(bytesRead < sizeof(size_t))
	{
		bytesRead += read(mySocket, buffer, sizeof(size_t) - bytesRead);
	}

	memcpy(length_p, buffer, sizeof(size_t));
	size_t length = *length_p;

	free(length_p);

	return length;
}

message_t receiveMessage(int mySocket)
{
	size_t length = readLength(mySocket);
	char* message = (char*) calloc(length + 1);
	char* buffer = (char*) calloc(length + 1);
	int byteCount = 0;

	while(byteCount < length)
	{
		int bytesRead = read(mySocket, buffer, length - bytesRead);
		byteCount += bytesRead;
		buffer[bytesRead] = '\0';
		strcat(message, buffer);
	}

	message_t parsedMessage = parseMessage(message);

	free(buffer);
	free(message);

	return parsedMessage;
}

void printMessage(message_t message)
{
	printf("%s:\n%s\n\n", message.sender, message.content);
}
