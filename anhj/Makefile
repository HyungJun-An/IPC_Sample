CC=gcc

SRC=./src
OUT=./bin

DIR_B_SOURCE=$(SRC)/DIR_B.c
DIR_C_SOURCE=$(SRC)/DIR_C.c
DIR_G_SOURCE=$(SRC)/DIR_G.c
DIR_H_SOURCE=$(SRC)/DIR_H.c
DIR_I_SOURCE=$(SRC)/DIR_I.c
SVR_SOURCE=$(SRC)/server.c
CLI_SOURCE=$(SRC)/client.c

DIR_B_TARGET=DIR_B
DIR_C_TARGET=DIR_C
DIR_G_TARGET=DIR_G
DIR_H_TARGET=DIR_H
DIR_I_TARGET=DIR_I
SVR_TARGET=server
CLI_TARGET=client

all:$(DIR_B_TARGET)\
	$(DIR_C_TARGET)\
	$(DIR_G_TARGET)\
	$(SVR_TARGET)\
	$(CLI_TARGET)\
	$(DIR_H_TARGET)\
	$(DIR_I_TARGET)

$(DIR_B_TARGET): $(DIR_B_SOURCE)
		@echo "Compile $^"
		$(CC) -o $(OUT)/$@ $^

$(DIR_C_TARGET): $(DIR_C_SOURCE)
		@echo "Compile $^"
		$(CC) -o  $(OUT)/$@ $^

$(DIR_G_TARGET): $(DIR_G_SOURCE)
		@echo "Compile $^"
		$(CC) -o  $(OUT)/$@ $^

$(DIR_H_TARGET): $(DIR_H_SOURCE)
		@echo "Compile $^"
		$(CC) -o  $(OUT)/$@ $^

$(DIR_I_TARGET): $(DIR_I_SOURCE)
		@echo "Compile $^"
		$(CC) -o  $(OUT)/$@ $^

$(SVR_TARGET): $(SVR_SOURCE)
	@echo "Compile $^"
	$(CC) -o  $(OUT)/$@ $^

$(CLI_TARGET): $(CLI_SOURCE)
	@echo "Compile $^"
	$(CC) -o  $(OUT)/$@ $^

clean:
		rm -f $(DIR_B_TARGET)\
			$(DIR_C_TARGET)\
			$(DIR_G_TARGET)\
			$(DIR_H_TARGET)\
			$(DIR_I_TARGET)\
			$(SVR_TARGET)\
			$(CLI_TARGET)

