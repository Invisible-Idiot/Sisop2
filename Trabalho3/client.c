#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "message.h"

char* readString(size_t maxSize)
{
	char* buffer = (char*) malloc(maxSize + 1);

	fgets(buffer, maxSize + 1, stdin);

	return buffer;
}

void readStringInto(char* str, size_t maxSize)
{
	char* buffer = readString(maxSize);

	strncpy(str, buffer, maxSize);

	free(buffer);
}

struct sockaddr_un localAddress()
{
	struct sockaddr_un address; address.sun_family = AF_UNIX; address.sun_path = /* alguma coisa */;

	return address;
}

int getSocket()
{
	int mySocket = socket(AF_UNIX, SOCK_STREAM, 0);

	socketAddress = remoteAddress();

	connect(mySocket, socketAddress, sizeof(sockerAddress));

	return mySocket;
}

void sendMessage(const char* myMessage, int mySocket)
{
	send(mySocket, myMessage, USRNAMESIZE + TEXTSIZE + 1, 0);
}

void client(void* nothing)
{
	int exit = 0;
	char* username;
	char* text = (char*) malloc(TEXTSIZE + 1);

	printf("Username (maximum %d characters):");

	username = readString(USRNAMESIZE);

	int mySocket = getSocket();

	printf("Hello, %s, welcome to chat.\n");

	while(!exit)
	{
		readStringInto(text);

		sendMessage(message(username, text), mySocket);
	}
}
