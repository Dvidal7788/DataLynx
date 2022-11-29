cc = gcc
BIN = bin

SRC = src
OBJ = obj
INCDIRS = . ./include

DEPFLAGS = -MP -MD
CFLAGS = -Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(DEPFLAGS)

CFILES = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(CFILES))
DEPFILES = $(patsubst $(SRC)/%.c,$(OBJ)/%.d,$(CFILES))

.PHONY: all
all:$(BIN)

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

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

-include $(DEPFILES)