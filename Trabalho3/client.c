#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
//#include <netinet/in.h>
#include "list.h"

#define SOCKET_ERROR -1
#define PORTNUMBER "4001"

#define TEST(x) fprintf(stderr, "%s\n", x);
#define PRINT(format, x) fprintf(stderr, format, x);

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
	struct addrinfo hints;
	struct addrinfo* serverInfo;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	int error = getaddrinfo(NULL, PORTNUMBER, &hints, &serverInfo);

	if(error) return SOCKET_ERROR;

	int mySocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	if(mySocket == SOCKET_ERROR) return SOCKET_ERROR;

	int status = connect(mySocket, serverInfo->ai_addr, serverInfo->ai_addrlen);

	freeaddrinfo(serverInfo);

	if(status == SOCKET_ERROR) return SOCKET_ERROR; else return mySocket;

/*
	struct addrinfo* res;
	int error = getaddrinfo(NULL, PORTNUMBER

	struct hostent* server = gethostbyname(hostname);
	int mySocket = socket(AF_INET, SOCK_STREAM, 0);

	if(mySocket == SOCKET_ERROR) return SOCKET_ERROR;

	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;     
	socketAddress.sin_port = htons(PORT);    
	socketAddress.sin_addr = *((struct in_addr *) server->h_addr);
	bzero(&(socketAddress.sin_zero), 8);

	int status = connect(mySocket, (struct sockaddr*) &socketAddress, sizeof(socketAddress));

	if(status == SOCKET_ERROR) return SOCKET_ERROR; else return mySocket;
*/
}

int main(int argc, char* argv[])
{
/*
	if(argc < 2)
	{
		printf("Please provide the host name for the server. Usage: client <hostname>\n");
		return 0;
	}

	char* hostname = argv[1];
*/
	int finished = 0;
	char* username;
	char* text = (char*) malloc(TEXTSIZE + 1);
	message_t receivedMessage;

	printf("Username (maximum %d characters):", USRNAMESIZE);

	username = readString(USRNAMESIZE);

	int mySocket = connectToServer();

	if(mySocket == SOCKET_ERROR)
	{
		printf("Could not connect to socket.\n");
		exit(0);
	}

	while(!finished)
	{
		receivedMessage = receiveMessage(mySocket);
//TEST("Client received message!")
		printMessage(receivedMessage);
//TEST("Client printed message!")
		readStringInto(text, TEXTSIZE);
//TEST("Client read input from user!")
		sendMessage(message(username, text), mySocket);
//TEST("Client sent message!")
	}

	close();

	free(username);
	free(text);

	exit(0);
}
