PROGRAM = dbl_lnk
cc = gcc
BIN = bin

SRC = src
OBJ = obj
INCDIRS = . ./include/

DEPFLAGS = -MP -MD
CFLAGS = -Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(DEPFLAGS)

CFILES = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(CFILES))
DEPFILES = $(patsubst $(SRC)/%.c,$(OBJ)/%.d,$(CFILES))

DATE = $(shell date +%F)
TIME = $(shell date +%r)

.PHONY: all
all:$(BIN)

$(OBJ)/%.o:$(SRC)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN):$(OBJS)
	$(CC) -o $@ $^
	@echo $(BIN) created: $(DATE) $(TIME)

dist:
	tar -czvf $(PROGRAM)_$(DATE).tgz *
	@echo $(PROGRAM)_$(DATE).tgz created: $(TIME)

staticlib: $(OBJ)/libdbl_lnk.o
	ar rcs ./include/lib$(PROGRAM)_static.a $<
	@echo ./include/lib$(PROGRAM)_static.a created successfully $(TIME)


.PHONY: run
run:
	./$(BIN)


.PHONY: clean
clean:
	rm -f $(BIN)
	rm -f *.o obj/*.o
	rm -f *.exe a.out
	@echo clean complete $(DATE) $(TIME)

-include $(DEPFILES)