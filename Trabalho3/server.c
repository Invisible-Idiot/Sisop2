#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "message.h"

#define MAXCONNECTIONS 8

list_t messages;

void addMessage(message_t message)
{
	addToList(messages, message);
}

listNode_t* lastMessage()
{
	return messages.last;
}

listNode_t* sendAwaitingMessages(listNode_t* lastSent, int mySocket)
{
	if(lastSent == NULL) return NULL;

	listNode_t* current = lastSent;
	listNode_t* next = lastSent->next;

	while(next != NULL)
	{
		current = next;
		next = next->next;

		sendMessage(message(current->message.sender, current->message.content), mySocket);
	}

	return current;
}

struct sockaddr_un localAddress()
{
	struct sockaddr_un address; address.sun_family = AF_UNIX; address.sun_path = /* alguma coisa */;

	return address;
}

int getSocket()
{
	int socket1 = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un* socketAddress = localAddress();

	int socket2 = bind(socket1, socketAddress, sizeof(socketAddress));

	listen(socket2, MAXCONNECTIONS);

	return socket2;
}

void connection(void* socket_p)
{
	int mySocket = *((int*) socket_p);
	listNode_t* lastMsg = lastMessage();
	int exit = 0;

	while(!exit)
	{
		message_t message = receiveMessage(mySocket);

		addMessage(parseMessage(message));

		lastMsg = sendAwaitingMessages(lastMsg, mySocket);
	}
}

void main()
{
	int socketId = *((int*) socket_p);

	while(!exit)
	{
		
	}
}
