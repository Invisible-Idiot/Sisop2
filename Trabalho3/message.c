#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "message.h"

char* message(const char* sender, const char* content)
{
	int totalSize = strlen(sender) + strlen(content) + 1 + 1;
	char* msg = (char*) malloc(totalSize);

	strcpy(msg, sender);
	strcat(msg, "#");
	strcat(msg, content);

	return msg;
}

message_t parseMessage(char* message)
{
	char* sender = strtok(message, "#");
	char* content = strtok(NULL, "#");

	message_t msg;
	msg.sender = sender;
	msg.content = content;

	return msg;
}

int* single(int x)
{
	int* p = (int*) malloc(sizeof(int));
	*p = x;

	return p;
}

void sendPacket(int mySocket, const void* packet, int len)
{
	int byteCount = 0;

	while(byteCount < len)
	{
		byteCount += send(mySocket, packet, len - byteCount, 0);
	}
}

void sendMessage(const char* myMessage, int mySocket)
{
	size_t* messageLength = (size_t*) malloc(sizeof(size_t));
	*messageLength = strlen(myMessage) + 1;

	sendPacket(mySocket, messageLength, sizeof(size_t));
	sendPacket(mySocket, myMessage, *messageLength);

//PRINT("Sent message: %s\n", myMessage)

	free(messageLength);
}

size_t receiveLength(int mySocket)
{
	size_t* length_p = (size_t*) malloc(sizeof(size_t));
	char buffer[sizeof(size_t)];
	char* temp = (char*) malloc(sizeof(size_t));
	int byteCount = 0;

	while(byteCount < sizeof(size_t))
	{
		int bytesRead = recv(mySocket, temp, sizeof(size_t) - byteCount, 0);
//if (bytesRead == -1) perror("read");
		memcpy(buffer + byteCount, temp, bytesRead);
		byteCount += bytesRead;
	}

	memcpy(length_p, buffer, sizeof(size_t));
	size_t length = *length_p;

	free(length_p);
	free(temp);

	return length;
}

message_t receiveMessage(int mySocket)
{
//TEST("Waiting for length..")
	size_t length = receiveLength(mySocket);
	char* message = (char*) calloc(length + 1, sizeof(char));
	char* buffer = (char*) calloc(length + 1, sizeof(char));
	int byteCount = 0;

	while(byteCount < length)
	{
		int bytesRead = recv(mySocket, buffer, length - byteCount, 0);

		byteCount += bytesRead;
		buffer[bytesRead] = '\0';
		strcat(message, buffer);
	}
//PRINT("Received message: %s\n", message)
	message_t parsedMessage = parseMessage(message);

	free(buffer);

	return parsedMessage;
}

void printMessage(message_t message)
{
	if(message.content != NULL)
		fprintf(stderr, "\n%s:\n%s\n\n", message.sender, message.content);
}
