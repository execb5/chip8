IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
CFLAGS = -Wall -g -I$(IDIR)
LIB_FLAGS = -lncurses

_DEPS = chip8.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = chip8.o main.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB_FLAGS)

.PHONY: clean run

run: main
	./main ${ARGS}
clean:
	rm -f $(ODIR)/*.o
	rm -f main

