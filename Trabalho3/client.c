#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "list.h"

#define SOCKET_ERROR -1

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

int connectToServer()
{
	int mySocket = socket(AF_UNIX, SOCK_STREAM, 0);

	if(mySocket == SOCKET_ERROR) return SOCKET_ERROR;

	struct sockaddr_un socketAddress;
	socketAddress.sun_family = AF_UNIX;
	strncpy(socketAddress.sun_path, "./private/channel", 125);

	int status = connect(mySocket, (struct sockaddr*) &socketAddress, sizeof(socketAddress));

	if(status == SOCKET_ERROR) return SOCKET_ERROR; else return mySocket;
}

int main(int argc, char* argv[])
{
/*
	if(argc < 2)
	{
		printf("Please provide the name of a file to serve as communication channel.\n");
		return 0;
	}

	char* filename = argv[1];

	if(strlen(filename) >= 126)
	{
		printf("File name must be at most 125 characters long.\n");
		return 0;
	}
*/
	int finished = 0;
	char* username;
	char* text = (char*) malloc(TEXTSIZE + 1);

	printf("Username (maximum %d characters):", USRNAMESIZE);

	username = readString(USRNAMESIZE);

	int mySocket = connectToServer();

	if(mySocket == SOCKET_ERROR)
	{
		printf("Could not connect to socket.\n");
		exit(0);
	}

	printf("Hello, %s, welcome to chat.\n", username);

	while(!finished)
	{
		printMessage(receiveMessage(mySocket));

		readStringInto(text, TEXTSIZE);

		sendMessage(message(username, text), mySocket);
	}

	close();

	free(username);
	free(text);

	exit(0);
}
