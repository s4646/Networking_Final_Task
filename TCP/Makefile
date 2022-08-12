.PHONY: all clean
CC = gcc
FLAGS = -g

all: client server nslookup web_client
	rm -f *.o

client: net_client.o
	$(CC) $(FLAGS) -o $@ $^

net_client.o: net_client.c
	$(CC) $(FLAGS) -c $^

server: net_server.o
	$(CC) $(FLAGS) -o $@ $^

net_server.o: net_server.c
	$(CC) $(FLAGS) -c $^

nslookup: nslookup.o
	$(CC) $(FLAGS) -o $@ $^

nslookup.o: nslookup.c
	$(CC) $(FLAGS) -c $^

web_client: web_client.o
	$(CC) $(FLAGS) -o $@ $^

web_client.o: web_client.c
	$(CC) $(FLAGS) -c $^

clean:
	rm -f *.a *.o *.so *.gch client server nslookup web_client