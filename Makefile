CC=gcc
CFLAGS=-g
TARGETS=sender receiver

all: $(TARGETS)

sender: sender.o
	$(CC) -o sender sender.o $(CFLAGS)

receiver: receiver.o
	$(CC) -o receiver receiver.o $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(TARGETS)

