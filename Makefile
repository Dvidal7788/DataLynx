cc = gcc
BIN = bin

SRC = src
OBJ = obj
# INCDIRS = . ./include

CFILES = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(CFILES))

.PHONY: all
all:$(BIN)

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -c $< -o $@

$(BIN):$(OBJS)
	$(CC) -o $@ $^


.PHONY: run
run:
	./$(BIN)


.PHONY: clean
clean:
	rm -f $(BIN)
	rm -f *.o obj/*
	rm -f *.exe a.out
