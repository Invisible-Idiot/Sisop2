#define TEXTSIZE 1024
#define USRNAMESIZE 32

#define TEST(x) fprintf(stderr, "%s\n", x);

struct message_s
{
	char* sender;
	char* content;
};

typedef struct message_s message_t;

char* message(const char* sender, const char* content);

message_t parseMessage(char* message);

int* single(int x);

void sendPacket(int mySocket, const void* packet, int len);

void sendMessage(const char* myMessage, int mySocket);

size_t receiveLength(int mySocket);

message_t receiveMessage(int mySocket);

void printMessage(message_t message);
