PROGRAM = csv_reader
CC = gcc
BIN = bin

SRC = src
OBJ = obj
INCDIRS = . ./include

CFILES = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o, $(CFILES))
DEPFILES = $(patsubst $(SRC)/%.c,$(OBJ)/%.d, $(CFILES))

DEPFLAGS = -MP -MD
CFLAGS = -Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(DEPFLAGS)

DATE = $(shell date +%F)
TIME = $(shell date +%r)

.PHONY: all
all:$(BIN)

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN):$(OBJS)
	$(CC) -o $@ $^

dist:
	rm -f *.tgz *.tar.gz
	tar -czvf $(PROGRAM)_$(DATE).tgz *
	@echo $(PROGRAM)_$(DATE).tgz created $(TIME)

staticlib: $(OBJ)/libcsv_reader.o
	rm -f include/*.a
	ar rcs ./include/lib$(PROGRAM)_static.a $<
	@echo ./include/lib$(PROGRAM)_static.a created successfully $(TIME)

.PHONY: clean
clean:
	rm -f *.o obj/*.o
	rm -f $(BIN)
	rm -f *.exe a.out
	rm -f vgcore*
	@echo clean complete $(DATE) $(TIME)

-include $(DEPFILES)
