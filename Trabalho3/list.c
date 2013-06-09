#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_t* newList()
{
	list_t* descriptor = (list_t*) malloc(sizeof(list_t));
	descriptor->first = NULL;
	descriptor->last = NULL;

	return descriptor;
}

listNode_t* newNode(message_t message)
{
	listNode_t* node = (listNode_t*) malloc(sizeof(listNode_t));

	node->message = message;
	node->next = NULL;

	return node;
}

listNode_t* addToList(list_t* list, message_t message)
{
	listNode_t* node = newNode(message);

	if(list->first == NULL)
	{
		list->first = node;
		list->last = node;
	}
	else
	{
		list->last->next = node;
		list->last = list->last->next;
	}

	return node;
}

void printList(list_t list)
{
	listNode_t* node = list.first;

	while(node != NULL)
		printf("%p :: ", node);

	printf("[]");
}
