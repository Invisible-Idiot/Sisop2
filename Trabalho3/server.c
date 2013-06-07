#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
//#include <netinet/in.h>
#include "list.h"

#define MAXCONNECTIONS 5
#define SOCKET_ERROR -1
#define PORTNUMBER "4001"

#define TEST(x) fprintf(stderr, "%s\n", x);
#define PRINT(format, x) fprintf(stderr, format, x);

list_t messages;

pthread_mutex_t mutex;

void addMessage(message_t message)
{
	pthread_mutex_lock(&mutex);

	addToList(messages, message);

	pthread_mutex_unlock(&mutex);
}

listNode_t* lastMessage()
{
	pthread_mutex_lock(&mutex);

	return messages.last;

	pthread_mutex_unlock(&mutex);
}

listNode_t* sendAwaitingMessages(listNode_t* lastSent, int mySocket)
{
	if(lastSent == NULL) return lastMessage();

	pthread_mutex_lock(&mutex);

	listNode_t* current = lastSent;
	listNode_t* next = lastSent->next;

	while(next != NULL)
	{
		current = next;
		next = next->next;

		sendMessage(message(current->message.sender, current->message.content), mySocket);
	}

	pthread_mutex_unlock(&mutex);

	return current;
}

int getSocket()
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

	int status = bind(mySocket, serverInfo->ai_addr, serverInfo->ai_addrlen);

	if(status == SOCKET_ERROR) return SOCKET_ERROR;

	status = listen(mySocket, MAXCONNECTIONS);

	freeaddrinfo(serverInfo);

	if(status == SOCKET_ERROR) return SOCKET_ERROR; else return mySocket;

/*
	int socket1 = socket(AF_UNIX, SOCK_STREAM, 0);

	if(socket1 == SOCKET_ERROR) return SOCKET_ERROR;

	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(PORTNUMBER);
	socketAddress.sin_addr.s_addr = INADDR_ANY;
	bzero(&(socketAddress.sin_zero), 8);

	int socket2 = bind(socket1, (struct sockaddr*) &socketAddress, sizeof(socketAddress));

	if(socket2 == SOCKET_ERROR) return SOCKET_ERROR;

	int status = listen(socket2, MAXCONNECTIONS);

	if(status == SOCKET_ERROR) return SOCKET_ERROR; else return socket2;
*/
}

int acceptConnection(int mySocket)
{
	struct sockaddr_storage clientAddress;
	socklen_t addressSize = sizeof(clientAddress);

	return accept(mySocket, (struct sockaddr*) &clientAddress, &addressSize);
}

void* connection(void* socket_p)
{
	int finished = 0;
	int mySocket = *((int*) socket_p);
	listNode_t* lastMsg = lastMessage();

	sendMessage(message("Server", "Hello and welcome to chat."), mySocket);

	while(!finished)
	{
		message_t message = receiveMessage(mySocket);

		addMessage(message);

		printMessage(message);

		lastMsg = sendAwaitingMessages(lastMsg, mySocket);
	}

	free(socket_p);

	pthread_exit(NULL);
}

void initMessages()
{
	messages.first = NULL;
	messages.last = NULL;
}

void main()
{
	initMessages();

	int mySocket = getSocket();
	int connectionSocket;

	if(mySocket == SOCKET_ERROR)
	{
		printf("Could not connect to socket.\n");
		exit(0);
	}

	pthread_t thread;

	pthread_mutex_init(&mutex, NULL);

	while(1)
	{
		connectionSocket = acceptConnection(mySocket);

		if(connectionSocket == SOCKET_ERROR)
		{
			printf("Could not accept connection from client.\n");
		}

		pthread_create(&thread, NULL, connection, single(connectionSocket));
	}

	close();

	exit(0);
}
