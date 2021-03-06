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
#define PORTNUMBER "4000"
#define USRONLINE " is online."
#define USRJOINED " joined the chat."
#define USRLEFT " left the chat."

list_t messages;
list_t users;

pthread_mutex_t messageMutex;
pthread_mutex_t userMutex;

listNode_t* addMessage(message_t message)
{
	listNode_t* node = NULL;

	pthread_mutex_lock(&messageMutex);
//TEST("Server locked mutex!")
	if(message.content != NULL)
	{
		node = addToList(&messages, message);
	}
//PRINT("Server added message: %s\n", message.content)
	pthread_mutex_unlock(&messageMutex);
//TEST("Server unlocked mutex!")

	return node;
}

int notExistsUser(char* username)
{
	pthread_mutex_lock(&messageMutex);

	int exists = 0;
	listNode_t* node = users.first;
	//fprintf(stderr, "entro");
	while(node != NULL)
	{
		//fprintf(stderr, "%s, %s", node->message.sender, username);
		if(strcmp(username, node->message.sender) == 0)
		{
			//fprintf(stderr, "existe");
			exists = 1;
			break;
		}
		node = node->next;
	}

	pthread_mutex_unlock(&messageMutex);
	
	return !exists;
}

int removeUser(char* username) // retorna 1 se o usuário estava na lista, 0 caso contrário
{
	pthread_mutex_lock(&userMutex);

	listNode_t* node1 = users.first;
	listNode_t* node2 = NULL;
	int removed = 0;

	while(node1)
	{
		if(0==strcmp(node1->message.sender, username))
		{
			if(node2 && node1->next!=NULL)
			{
				node2->next = node1->next;
				removed = 1;
			}
			else if(node2 && node1->next == NULL)
			{
				users.last = node2;
				node2->next = NULL;
				removed = 1;
			}
			else if(node2 == NULL && node1->next == NULL)
			{
				users.first = NULL;
				users.last = NULL;
				removed = 1;
			}
			else //node2 == NULL && node1->next != NULL
			{
				users.first = node1->next;
				removed = 1;
			}

			free(node1);
		}
		else
		{
			node2 = node1;
			node1 = node1->next;
		}
	}

	pthread_mutex_unlock(&userMutex);

	return removed;
		
}

int addUser(char* username)
{
	if(notExistsUser(username))
	{
		message_t user;
		user.sender = username;
		user.content = NULL;

		pthread_mutex_lock(&userMutex);

		addToList(&users, user);

		pthread_mutex_unlock(&userMutex);
		return 1;
	}
	else
	{
		/*listNode_t* node = users.first;
		while(node != NULL)
		{
			fprintf(stderr, "%s\n", node->message.sender);
			node = node->next;
		}*/
		return 0;
	}
	
}

void sendUsersOnline(int mySocket)
{
	pthread_mutex_lock(&userMutex);

	listNode_t* current = users.first;
	char* msg;

	while(current != NULL)
	{
		msg = malloc(strlen(current->message.sender) + strlen(USRONLINE) + 1);
		sprintf(msg, "%s%s", current->message.sender, USRONLINE);
		sendMessage(message("_", msg), mySocket);
		free(msg);
		current = current->next;
	}

	sendMessage(message("_", ""), mySocket);

	pthread_mutex_unlock(&userMutex);
}

message_t userActionMessage(char* username, const char* action)
{
	char* msg = malloc(strlen(username) + strlen(action) + 1);
	sprintf(msg, "%s%s", username, action);

	return parseMessage(message("_", msg));
}

message_t userJoinedMessage(char* username)
{
	return userActionMessage(username, USRJOINED);
}

message_t userLeftMessage(char* username)
{
	return userActionMessage(username, USRLEFT);
}

void printMessages()
{
	pthread_mutex_lock(&messageMutex);

	printList(messages);

	pthread_mutex_unlock(&messageMutex);
}

listNode_t* lastMessage()
{
	pthread_mutex_lock(&messageMutex);

	listNode_t* last = messages.last;

	pthread_mutex_unlock(&messageMutex);

	return last;
}

listNode_t* sendAwaitingMessages(listNode_t* lastSent, int mySocket)
{
	listNode_t* current;
	listNode_t* next;

	pthread_mutex_lock(&messageMutex);

	current = lastSent;
	next = lastSent->next;

	while(next != NULL)
	{
		current = next;
		next = next->next;

		sendMessage(message(current->message.sender, current->message.content), mySocket);
//PRINT("Sending \"%s\"\n", current->message.content)
	}

	sendMessage(message("_", ""), mySocket);

	pthread_mutex_unlock(&messageMutex);

	return current;
}

int getSocket()
{
	struct addrinfo hints;
	struct addrinfo* serverInfo;
	int yes = 1;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	int error = getaddrinfo(NULL, PORTNUMBER, &hints, &serverInfo);

	if(error) return SOCKET_ERROR;

	int mySocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	if(mySocket == SOCKET_ERROR) return SOCKET_ERROR;

	setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

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
	listNode_t* lastMsg;
	message_t msg;

	free(socket_p);

	msg = receiveMessage(mySocket);

	if(!addUser(msg.sender))
	{
		//printMessage(errorMsg);
		//addMessage(errorMsg);
		sendMessage(message("_", USER_ALREADY_EXISTS), mySocket);
		sendMessage(message("_", ""), mySocket);
		finished = 1;
	}
	else
	{
		lastMsg = addMessage(userJoinedMessage(msg.sender));
		printMessage(msg);
		sendMessage(message("_","Hello, welcome to chat."), mySocket);
		sendUsersOnline(mySocket);
	}
	
	while(!finished)
	{
		msg = receiveMessage(mySocket);

		if(msg.content != NULL && 0==strcmp(msg.content,EXIT_MESSAGE))
		{
			removeUser(msg.sender);
			addMessage(userLeftMessage(msg.sender));
			printMessage(msg);
			finished = 1;
		}
		else
		{
			addMessage(msg);
//TEST("Server added message to list!")
			printMessage(msg);
//TEST("Server printed message!")
			lastMsg = sendAwaitingMessages(lastMsg, mySocket);
//TEST("Server sent awaiting messages!")
		}
	}

	shutdown(mySocket, 2);

	pthread_exit(NULL);
}

void initMessages()
{
	messages.first = NULL;
	messages.last = NULL;
}

int main()
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

	pthread_mutex_init(&messageMutex, NULL);

	while(1)
	{
		connectionSocket = acceptConnection(mySocket);

		if(connectionSocket == SOCKET_ERROR)
		{
			printf("Could not accept connection from client.\n");
		}

		pthread_create(&thread, NULL, connection, single(connectionSocket));
	}

	shutdown(mySocket, 2);

	exit(0);
}
