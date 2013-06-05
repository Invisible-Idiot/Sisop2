#include <stdlib.h>
#include "message.h"

struct listNode_s
{
	message_t this;
	struct listNode_s* next;
}

struct list_s
{
	struct listNode_s* first;
	struct listNode_s* last;
}

typedef struct listNode_s listNode_t;
typedef struct list_s list_t;

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

	node->this = message;
	node->next = NULL;

	return list;
}

void addToList(list_t* list, message_t message)
{
	if(list->first == NULL)
	{
		list->first = message;
		list->last = message;
	}
	else
	{
		list->last->next = newNode(message);
		list->last = list->last->next;
	}
}
