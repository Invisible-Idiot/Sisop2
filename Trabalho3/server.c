#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "list.h"

#define MAXCONNECTIONS 8
#define SOCKET_ERROR -1

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
	if(lastSent == NULL) return NULL;

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
	int socket1 = socket(AF_UNIX, SOCK_STREAM, 0);

	if(socket1 == SOCKET_ERROR) return SOCKET_ERROR;

	struct sockaddr_un socketAddress;
	socketAddress.sun_family = AF_UNIX;
	strncpy(socketAddress.sun_path, "./private/channel", 125);

	int socket2 = bind(socket1, (struct sockaddr*) &socketAddress, sizeof(socketAddress));

	if(socket2 == SOCKET_ERROR) return SOCKET_ERROR;

	int status = listen(socket2, MAXCONNECTIONS);

	if(status == SOCKET_ERROR) return SOCKET_ERROR; else return socket2;
}

int acceptConnection(int mySocket)
{
	socklen_t addressSize;
	struct sockaddr_un socketAddress;

	return accept(mySocket, (struct sockaddr*) &socketAddress, &addressSize);
}

void* connection(void* socket_p)
{
	int finished = 0;
	int mySocket = *((int*) socket_p);
	listNode_t* lastMsg = lastMessage();

	while(!finished)
	{
		message_t message = receiveMessage(mySocket);

		addMessage(message);

		lastMsg = sendAwaitingMessages(lastMsg, mySocket);
	}

	free(socket_p);

	pthread_exit(NULL);
}

void main()
{
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
			break;
		}

		pthread_create(&thread, NULL, connection, single(acceptConnection(mySocket)));
	}

	close();

	exit(0);
}
