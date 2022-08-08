.PHONY: all clean
CC = gcc
FLAGS = -g
PROB1 = send recv
PROB2 = source sink gateway

all: $(PROB1) $(PROB2)
	rm -f *.o

send: send_udp.o
	$(CC) $(FLAGS) -o $@ $^

send_udp.o: send_udp.c
	$(CC) $(FLAGS) -c $^

recv: recv_udp.o
	$(CC) $(FLAGS) -o $@ $^

recv_udp.o: recv_udp.c
	$(CC) $(FLAGS) -c $^

source: source.o
	$(CC) $(FLAGS) -o $@ $^

source.o: source.c
	$(CC) $(FLAGS) -c $^

sink: sink.o
	$(CC) $(FLAGS) -o $@ $^

sink.o: sink.c
	$(CC) $(FLAGS) -c $^

gateway: gateway.o
	$(CC) $(FLAGS) -o $@ $^

gateway.o: gateway.c
	$(CC) $(FLAGS) -c $^

clean:
	rm -f *.a *.o *.so *.gch $(PROB1) $(PROB2)