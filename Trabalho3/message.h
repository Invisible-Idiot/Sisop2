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

size_t single(int x);

void sendMessage(const char* myMessage, int mySocket);

size_t receiveLength(int mySocket);

message_t receiveMessage(int mySocket);

void printMessage(message_t message);
