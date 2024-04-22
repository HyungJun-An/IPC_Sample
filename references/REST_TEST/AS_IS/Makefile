CC 	= gcc
#CFLAGS 	= -Wall -Wextra -g
CFLAGS = -g -DHAVE_STDINT_H

INC_PATH= ./includes

INC = -I$(INC_PATH)/
LIBS = -L./libs -lpthread -lrt -ldl 
DB_LIBS= -L./libs -lmysqlclient -lcps2010 -lpthread -lrt -ldl
SRC = ./src

OUT = ./bin

CH_SOURCE = $(SRC)/CH_Pro.c
CH_TARGET = CH 
AP_SOURCE = $(SRC)/AP.c
AP_TARGET = AP 
 

all: 	$(CH_TARGET)\
	$(AP_TARGET)

$(CH_TARGET): $(CH_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(AP_TARGET): $(AP_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

clean:
	rm -f 	$(CH_TARGET)		\
	    $(AP_TARGET)
