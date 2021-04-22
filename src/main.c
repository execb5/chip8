#include <stdio.h>
#include "chip8.h"

#define NUMBER_OF_ARGUMENTS 2

int main(int argc, char** argv) {
	if (argc < NUMBER_OF_ARGUMENTS + 1) {
		printf("Wrong number of arguments.\n");
		printf("Expected %d, but got %d\n", NUMBER_OF_ARGUMENTS, argc - 1);
		return 1;
	}
	Chip8* a = start(argv[1]);
	dump_memory_to_file(a, argv[2]);
	op_00e0(a);
	op_00ee(a);
	return 0;
}
