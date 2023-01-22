#include <stdio.h>
#include "../include/chip8.h"

#define NUMBER_OF_ARGUMENTS 2

int main(int argc, char** argv) {
	if (argc < NUMBER_OF_ARGUMENTS + 1) {
		printf("Wrong number of arguments.\n");
		printf("Expected %d, but got %d\n", NUMBER_OF_ARGUMENTS, argc - 1);
		return 1;
	}
	Chip8* a = create();
	load_rom(a, argv[1]);
	dump_memory_to_file(a, argv[2]);
	op_00e0(a);
	op_00ee(a);
	op_1nnn(a);
	op_2nnn(a);
	op_3xkk(a);
	op_4xkk(a);
	op_5xy0(a);
	op_6xkk(a);
	op_7xkk(a);
	op_8xy0(a);
	op_8xy1(a);
	op_8xy2(a);
	op_8xy3(a);
	op_8xy4(a);
	op_8xy5(a);
	op_8xy6(a);
	op_8xy7(a);
	op_8xye(a);
	op_9xy0(a);
	op_annn(a);
	op_bnnn(a);
	op_cxkk(a, generate_random_byte);
	op_dxyn(a);
	op_ex9e(a);
	op_exa1(a);
	op_fx07(a);
	op_fx0a(a);
	op_fx15(a);
	destroy(a);
	return 0;
}
