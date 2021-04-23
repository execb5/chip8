IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
CFLAGS = -Wall -g -I$(IDIR)
LIB_FLAGS = -lncurses
TEST_FLAGS = -lcmocka

_DEPS = chip8.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = chip8.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_MAIN = main.o

MAIN = $(patsubst %,$(ODIR)/%,$(_MAIN))

_TEST = test.o

TEST = $(patsubst %,$(ODIR)/%,$(_TEST))

all: main test

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ) $(MAIN)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB_FLAGS)

test: $(OBJ) $(TEST)
	$(CC) -o $@ $^ $(CFLAGS) $(TEST_FLAGS)

.PHONY: clean run

run: main
	./main ${ARGS}

run_test: test
	./test

clean:
	rm -f $(ODIR)/*.o
	rm -f main
	rm -f test

