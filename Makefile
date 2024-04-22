CC=gcc
CFLAGS=-Wall -Wextra -g

SRC=./src
OUT=./bin
INC=./include

SVR_SOURCE=$(SRC)/poll.c
CLI_SOURCE=$(SRC)/client.c

SVR_TARGET=server
CLI_TARGET=client

all:$(SVR_TARGET)\
	$(CLI_TARGET)

$(SVR_TARGET): $(SVR_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^
$(CLI_TARGET): $(CLI_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^

clean:
	rm -f $(SVR_TARGET)\
		$(CLI_TARGET)
