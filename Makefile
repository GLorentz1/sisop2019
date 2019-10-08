CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: regra1 regra2

regra1:
	$(CC) -c $(SRC_DIR)/lib.c -o $(BIN_DIR)/lib.o -Wall

regra2:
	ar rcs $(LIB_DIR)/libcthread.a $(BIN_DIR)/*.o

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/lib.o ./testes/*.run
