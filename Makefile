PROGRAM = csv_reader
CC = gcc
BIN = bin

SRC = src
OBJ = obj
INCDIRS = . ./include

CFILES = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c,./%.o, $(CFILES))
DEPFILES = $(patsubst $(SRC)/%.c,$(OBJ)/%.d, $(CFILES))

CFLAGS = -Wall -Wextra -g $(DEPFILES)

all:
	$(CC) -c $(CFILES)
	$(CC) -o $(BIN) $(OBJS)

clean:
	rm -f obj/*.o
	rm $(BIN)
