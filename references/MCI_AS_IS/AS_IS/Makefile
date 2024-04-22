CC 	= gcc
#CFLAGS 	= -Wall -Wextra -g
CFLAGS = -g -DHAVE_STDINT_H

INC_PATH= ./includes

INC = -I$(INC_PATH)/
LIBS = -L./libs -lpthread -lrt -ldl 
DB_LIBS= -L./libs -lmysqlclient -lcps2010 -lpthread -lrt -ldl
SRC = ./src

OUT = ./bin

A_SOURCE =$(SRC)/MCI_A.c
A_TARGET = MCI_A 
B_SOURCE = $(SRC)/DIR_B.c
B_TARGET = DIR_B 
BTEST_SOURCE = $(SRC)/DIR_Btest.c
BTEST_TARGET = DIR_Btest 
ITEST_SOURCE = $(SRC)/FEP_Itest.c
ITEST_TARGET = FEP_Itest 
ATEST_SOURCE = $(SRC)/MCI_Through.c
ATEST_TARGET = MCI_Through 
C_SOURCE = $(SRC)/DIR_C.c
C_TARGET = DIR_C 
G_SOURCE = $(SRC)/FEP_G.c
G_TARGET = FEP_G
H_SOURCE = $(SRC)/FEP_H.c
H_TARGET = FEP_H
I_SOURCE = $(SRC)/FEP_I.c
I_TARGET = FEP_I
J_SOURCE = $(SRC)/FEP_J.c
J_TARGET = FEP_J
K_SOURCE = $(SRC)/FEP_K.c
K_TARGET = FEP_K
L_SOURCE = $(SRC)/DIR_L.c
L_TARGET = DIR_L
M_SOURCE = $(SRC)/DIR_M.c
M_TARGET = DIR_M
T_SOURCE = $(SRC)/MCI_test.c
T_TARGET = MCI_test 
 

all: 	$(A_TARGET)\
	$(B_TARGET)\
	$(BTEST_TARGET)\
	$(ITEST_TARGET)\
	$(ATEST_TARGET)\
	$(C_TARGET)\
	$(G_TARGET)\
	$(H_TARGET)\
	$(I_TARGET)\
	$(J_TARGET)\
	$(K_TARGET)\
	$(L_TARGET)\
	$(M_TARGET)

$(T_TARGET): $(T_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(A_TARGET): $(A_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(B_TARGET): $(B_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(BTEST_TARGET): $(BTEST_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(ITEST_TARGET): $(ITEST_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(ATEST_TARGET): $(ATEST_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(C_TARGET): $(C_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(G_TARGET): $(G_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(H_TARGET): $(H_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(I_TARGET): $(I_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(J_TARGET): $(J_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(K_TARGET): $(K_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(L_TARGET): $(L_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

$(M_TARGET): $(M_SOURCE)
	@echo "Compile $^"
	$(CC) $(CFLAGS) -o $(OUT)/$@ $^ $(INC) $(LIBS)

clean:
	rm -f 	$(A_TARGET)		\
            $(B_TARGET)		\
            $(BTEST_TARGET)		\
            $(ITEST_TARGET)		\
	    $(C_TARGET)		\	
	    $(G_TARGET)		\
	    $(H_TARGET)		\
	    $(I_TARGET)		\
	    $(J_TARGET)		\
	    $(K_TARGET)		\
	    $(L_TARGET)		\
	    $(M_TARGET)
