CC 	= gcc
#CFLAGS 	= -Wall -Wextra -g
CFLAGS = -g -DHAVE_STDINT_H

INC_PATH= ./includes

INC = -I$(INC_PATH)/
LIBS = -L./libs -lpthread -lrt -ldl -lcurl
SRC = ./src

OUT = ./bin

CLIENT_SOURCE = $(SRC)/client_http.c
CLIENT_TARGET = client 
SERVER_SOURCE = $(SRC)/server_http.c
SERVER_TARGET = server
 

all: 	$(CLIENT_TARGET)\
	$(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(SERVER_TARGET): $(SERVER_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

clean:
	rm -f 	$(CLIENT_TARGET)		\
	    $(SERVER_TARGET)
