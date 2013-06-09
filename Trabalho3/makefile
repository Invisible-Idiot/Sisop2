chat: client server

client: message.c list.c client.c
	gcc -pthread -o client message.c list.c client.c -Wall

server: message.c list.c server.c
	gcc -pthread -o server message.c list.c server.c -Wall

clean:
	rm client server
