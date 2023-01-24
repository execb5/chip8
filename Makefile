IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
CFLAGS = -Wall -g -I$(IDIR) -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
LIB_FLAGS = -L/opt/homebrew/lib -lSDL2
TEST_FLAGS = -lcmocka

_DEPS = chip8.h instructions.h platform.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = chip8.o instructions.o platform.o

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
	$(CC) -o $@ $^ $(CFLAGS) $(LIB_FLAGS) $(TEST_FLAGS)

.PHONY: clean run run_test

run: main
	./main ${ARGS}

run_test: test
	./test

clean:
	rm -f $(ODIR)/*.o
	rm -f main
	rm -f test
	rm -f *.hex
