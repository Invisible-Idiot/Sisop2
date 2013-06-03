#define TEXTSIZE 1024
#define USRNAMESIZE 32

struct message_s
{
	char* sender;
	char* content;
};

typedef struct message_s message_t;

char* message(char* sender, char* content);
message_t parseMessage(char* message);
