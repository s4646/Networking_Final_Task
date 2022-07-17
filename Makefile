.PHONY: all clean
CC = gcc
FLAGS = -g
OBJ_RECV = recv_udp.o
OBJ_SEND = send_udp.o
OBJ_SRC = source.o
OBJ_SINK = sink.o
OBJ_GATE = gateway.o
PROB1 = send recv
PROB2 = source sink gateway

all: $(PROB1) $(PROB2)
	rm -f *.o

send: send_udp.o
	$(CC) $(FLAGS) -o send $(OBJ_SEND)

send_udp.o: send_udp.c
	$(CC) $(FLAGS) -c send_udp.c

recv: recv_udp.o
	$(CC) $(FLAGS) -o recv $(OBJ_RECV)

recv_udp.o: recv_udp.c
	$(CC) $(FLAGS) -c recv_udp.c

source: source.o
	$(CC) $(FLAGS) -o source $(OBJ_SRC)

source.o: source.c
	$(CC) $(FLAGS) -c source.c

sink: sink.o
	$(CC) $(FLAGS) -o sink $(OBJ_SINK)

sink.o: sink.c
	$(CC) $(FLAGS) -c sink.c

gateway: gateway.o
	$(CC) $(FLAGS) -o gateway $(OBJ_GATE)

gateway.o: gateway.c
	$(CC) $(FLAGS) -c gateway.c

clean:
	rm -f *.a *.o *.so *.gch $(PROB1) $(PROB2)