#include "message.h"

struct listNode_s
{
	message_t message;
	struct listNode_s* next;
};

struct list_s
{
	struct listNode_s* first;
	struct listNode_s* last;
};

typedef struct listNode_s listNode_t;
typedef struct list_s list_t;

list_t* newList();

listNode_t* newNode(message_t message);

void addToList(list_t* list, message_t message);

void printList(list_t list);
