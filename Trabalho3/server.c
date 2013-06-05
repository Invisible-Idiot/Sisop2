#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "message.h"

#define MAXCONNECTIONS 8

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

struct sockaddr_un getAddress()
{
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strncpy(address.sun_path, "./private/channel", 125);

	return address;
}

int getSocket()
{
	int socket1 = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un* socketAddress = getAddress();

	int socket2 = bind(socket1, socketAddress, sizeof(socketAddress));

	listen(socket2, MAXCONNECTIONS);

	return socket2;
}

int accept(int mySocket)
{
	struct sockaddr_un* socketAddress = (struct sockaddr_un*) malloc(sizeof(struct sockaddr_un));

	return accept(mySocket, socketAddress, sizeof(socketAddress));
}

void connection(void* socket_p)
{
	int exit = 0;
	int mySocket = *((int*) socket_p);
	listNode_t* lastMsg = lastMessage();

	while(!exit)
	{
		message_t message = receiveMessage(mySocket);

		addMessage(parseMessage(message));

		lastMsg = sendAwaitingMessages(lastMsg, mySocket);
	}

	free(socket_p);

	pthread_exit(NULL);
}

void main()
{
	int mySocket = getSocket();
	pthread_t thread;

	pthread_mutex_init(&mutex, NULL);

	while(true)
	{
		pthread_create(&thread, NULL, connection, single(accept(mySocket)));
	}
}
