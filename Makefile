all: server.c client.c
	g++ -o server server.c
	g++ -o client client.c

clean:
	rm -f server client
