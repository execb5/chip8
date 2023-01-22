#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/chip8.h"

static uint16_t start_address = 0x0200;
static uint16_t end_address = 0x0fff;
static uint16_t font_set_start_address = 0x0050;

static uint8_t font_set_size = 80;
static uint8_t font_set[80] = {
	0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
	0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
	0x90, 0x90, 0xf0, 0x10, 0x10, // 4
	0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
	0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
	0xf0, 0x10, 0x20, 0x40, 0x40, // 7
	0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
	0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
	0xf0, 0x90, 0xf0, 0x90, 0x90, // A
	0xe0, 0x90, 0xe0, 0x90, 0xe0, // B
	0xf0, 0x80, 0x80, 0x80, 0xf0, // C
	0xe0, 0x90, 0x90, 0x90, 0xe0, // D
	0xf0, 0x80, 0xf0, 0x80, 0xf0, // E
	0xf0, 0x80, 0xf0, 0x80, 0x80  // F
};

Chip8* create(void) {
	Chip8* a = calloc(1, sizeof(Chip8));

	if (!a) {
		exit(2);
	}

	memcpy(&a->memory[font_set_start_address], font_set, font_set_size);

	return a;
}

Chip8* load_rom(Chip8* chip, char* rom_name) {
	FILE* f = fopen(rom_name, "r");
	if (!f) {
		exit(1);
	}
	fread(&chip->memory[start_address], end_address - start_address, 1, f);
	fclose(f);

	return chip;
}

void dump_memory_to_file(Chip8* chip, char* memory_file_name) {
	FILE* f = fopen(memory_file_name, "wb");
	if (!f) {
		exit(1);
	}
	fwrite(chip->memory, sizeof(uint8_t), CHIP8_MEMORY_SIZE, f);
	fclose(f);
}

void op_00e0(Chip8* chip) {
	memset(chip->video, 0, CHIP8_PIXEL_COUNT);
}

void op_00ee(Chip8* chip) {
	chip->pc = chip->stack[--chip->sp];
}

void op_1nnn(Chip8* chip) {
	uint16_t address = chip->opcode & 0x0fffu;
	chip->pc = address;
}

void op_2nnn(Chip8* chip) {
	uint16_t address = chip->opcode & 0x0fffu;

	chip->stack[chip->sp] = chip->pc;
	chip->sp++;
	chip->pc = address;
}

void op_3xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	if (chip->registers[vx] == kk) {
		chip->pc += 2;
	}
}

void op_4xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	if (chip->registers[vx] != kk) {
		chip->pc += 2;
	}
}

void op_5xy0(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vx] == chip->registers[vy]) {
		chip->pc += 2;
	}
}

void op_6xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	chip->registers[vx] = kk;
}

void op_7xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	chip->registers[vx] += kk;
}

void op_8xy0(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] = chip->registers[vy];
}

void op_8xy1(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] |= chip->registers[vy];
}

void op_8xy2(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] &= chip->registers[vy];
}

void op_8xy3(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] ^= chip->registers[vy];
}

void op_8xy4(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	uint16_t sum = chip->registers[vx] + chip->registers[vy];

	if (sum > 0xffu) {
		chip->registers[0xf] = 1;
	} else {
		chip->registers[0xf] = 0;
	}

	chip->registers[vx] = sum & 0x00ffu;
}

void op_8xy5(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vx] > chip->registers[vy]) {
		chip->registers[0xf] = 1;
	} else {
		chip->registers[0xf] = 0;
	}

	chip->registers[vx] -= chip->registers[vy];
}

void op_8xy6(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->registers[0xf] = (chip->registers[vx] & 0x0001u);

	chip->registers[vx] >>= 1;
}

void op_8xy7(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vy] > chip->registers[vx]) {
		chip->registers[0xf] = 1;
	} else {
		chip->registers[0xf] = 0;
	}

	chip->registers[vx] = chip->registers[vy] - chip->registers[vx];
}

void op_8xye(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	uint8_t most_significant_bit_of_vx = chip->registers[vx] >> 7u;
	chip->registers[0xf] = most_significant_bit_of_vx;

	chip->registers[vx] <<= 1;
}

void op_9xy0(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vx] != chip->registers[vy]) {
		chip->pc += 2;
	}
}

void op_annn(Chip8* chip) {
	chip->index = chip->opcode & 0x0fffu;
}

void op_bnnn(Chip8* chip) {
	chip->pc = chip->registers[0x0] + (chip->opcode & 0x0fffu);
}

void op_cxkk(Chip8* chip, uint8_t (*byte_generator_function)()) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = chip->opcode & 0x00ffu;

	uint8_t random_byte = (*byte_generator_function)();

	chip->registers[vx] = random_byte & kk;
}

void op_dxyn(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;
	uint8_t n = chip->opcode & 0x000fu;

	uint8_t x_start = chip->registers[vx] % CHIP8_SCREEN_WIDTH;
	uint8_t y_start = chip->registers[vy] % CHIP8_SCREEN_HEIGHT;

	chip->registers[0xf] = 0x00;

	for (uint8_t row = 0; row < n; row++) {
		uint8_t sprite_byte = chip->memory[chip->index + row];

		for (int column = 0; column < 8; ++column) {
			uint8_t sprite_pixel = sprite_byte & (0x80u >> column);
			uint8_t* screen_pixel = &chip->video[(y_start + row) * CHIP8_SCREEN_WIDTH + (x_start + column)];

			if (sprite_pixel) {
				if (*screen_pixel == 0xff) {
					chip->registers[0xf] = 0x01;
				}

				*screen_pixel ^= 0xff;
			}
		}
	}
}

void op_ex9e(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0F00u) >> 8u;

	uint8_t key = chip->registers[vx];

	if (chip->keypad[key]) {
		chip->pc += 2;
	}
}

void op_exa1(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0F00u) >> 8u;

	uint8_t key = chip->registers[vx];

	if (!chip->keypad[key]) {
		chip->pc += 2;
	}
}

void op_fx07(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0F00u) >> 8u;

	chip->registers[vx] = chip->delayTimer;
}

void op_fx0a(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0F00u) >> 8u;

	if (chip->keypad[0x0]) {
		chip->registers[vx] = 0x0;
	} else if (chip->keypad[0x1]) {
		chip->registers[vx] = 0x1;
	} else if (chip->keypad[0x2]) {
		chip->registers[vx] = 0x2;
	} else if (chip->keypad[0x3]) {
		chip->registers[vx] = 0x3;
	} else if (chip->keypad[0x4]) {
		chip->registers[vx] = 0x4;
	} else if (chip->keypad[0x5]) {
		chip->registers[vx] = 0x5;
	} else if (chip->keypad[0x6]) {
		chip->registers[vx] = 0x6;
	} else if (chip->keypad[0x7]) {
		chip->registers[vx] = 0x7;
	} else if (chip->keypad[0x8]) {
		chip->registers[vx] = 0x8;
	} else if (chip->keypad[0x9]) {
		chip->registers[vx] = 0x9;
	} else if (chip->keypad[0xa]) {
		chip->registers[vx] = 0xa;
	} else if (chip->keypad[0xb]) {
		chip->registers[vx] = 0xb;
	} else if (chip->keypad[0xc]) {
		chip->registers[vx] = 0xc;
	} else if (chip->keypad[0xd]) {
		chip->registers[vx] = 0xd;
	} else if (chip->keypad[0xe]) {
		chip->registers[vx] = 0xe;
	} else if (chip->keypad[0xf]) {
		chip->registers[vx] = 0xf;
	} else {
		chip->opcode -= 2;
	}
}

void op_fx15(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0F00u) >> 8u;

	chip->delayTimer = chip->registers[vx];
}

void op_fx18(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0F00u) >> 8u;

	chip->soundTimer = chip->registers[vx];
}

void destroy(Chip8* chip) {
	free(chip);
}

uint8_t generate_random_byte(void) {
	uint8_t random_byte;
	FILE* dev_random = fopen("/dev/random", "r");
	fread(&random_byte, sizeof(uint8_t), 1, dev_random);
	fclose(dev_random);

	return random_byte;
}
