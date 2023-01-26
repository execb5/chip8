#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/chip8.h"
#include "../inc/instructions.h"

static uint16_t start_address = 0x0200;
static uint16_t end_address = 0x0fff;

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

static void (*opcode_table[0x10])(Chip8*) = {0};
static void (*table_zero[0xf])(Chip8*) = {0};
static void (*table_eight[0xf])(Chip8*) = {0};
static void (*table_e[0xf])(Chip8*) = {0};
static void (*table_f[0x66])(Chip8*) = {0};

static void zero(Chip8* chip) {
	table_zero[chip->opcode & 0x000fu](chip);
}

static void eight(Chip8* chip) {
	table_eight[chip->opcode & 0x000fu](chip);
}

static void e(Chip8* chip) {
	table_e[chip->opcode & 0x000fu](chip);
}

static void f(Chip8* chip) {
	table_f[chip->opcode & 0x00ffu](chip);
}

static void cxkk(Chip8* chip) {
	op_cxkk(chip, generate_random_byte);
}

static void op_null(Chip8* chip) {}

static void initialize(void) {
	opcode_table[0x0] = &zero;
	opcode_table[0x1] = &op_1nnn;
	opcode_table[0x2] = &op_2nnn;
	opcode_table[0x3] = &op_3xkk;
	opcode_table[0x4] = &op_4xkk;
	opcode_table[0x5] = &op_5xy0;
	opcode_table[0x6] = &op_6xkk;
	opcode_table[0x7] = &op_7xkk;
	opcode_table[0x8] = &eight;
	opcode_table[0x9] = &op_9xy0;
	opcode_table[0xa] = &op_annn;
	opcode_table[0xb] = &op_bnnn;
	opcode_table[0xc] = &cxkk;
	opcode_table[0xd] = &op_dxyn;
	opcode_table[0xe] = &e;
	opcode_table[0xf] = &f;

	for (uint8_t i = 0; i < 0xf; i++) {
		table_zero[i] = &op_null;
		table_eight[i] = &op_null;
		table_e[i] = &op_null;
	}

	table_zero[0x0] = &op_00e0;
	table_zero[0xe] = &op_00ee;

	table_eight[0x0] = &op_8xy0;
	table_eight[0x1] = &op_8xy1;
	table_eight[0x2] = &op_8xy2;
	table_eight[0x3] = &op_8xy3;
	table_eight[0x4] = &op_8xy4;
	table_eight[0x5] = &op_8xy5;
	table_eight[0x6] = &op_8xy6;
	table_eight[0x7] = &op_8xy7;
	table_eight[0xe] = &op_8xye;

	table_e[0x1] = &op_exa1;
	table_e[0xe] = &op_ex9e;

	for (uint8_t i = 0; i < 0x66; i++) {
		table_f[i] = &op_null;
	}

	table_f[0x07] = &op_fx07;
	table_f[0x0a] = &op_fx0a;
	table_f[0x15] = &op_fx15;
	table_f[0x18] = &op_fx18;
	table_f[0x1e] = &op_fx1e;
	table_f[0x29] = &op_fx29;
	table_f[0x33] = &op_fx33;
	table_f[0x55] = &op_fx55;
	table_f[0x65] = &op_fx65;
}
Chip8* create(void) {
	Chip8* a = calloc(1, sizeof(Chip8));

	if (!a) {
		exit(2);
	}

	memcpy(&a->memory[CHIP8_FONT_SET_START_ADDRESS], font_set, font_set_size);

	a->pc = start_address;

	return a;
}

void load_rom(Chip8* chip, char* rom_name) {
	FILE* f = fopen(rom_name, "r");
	if (!f) {
		exit(1);
	}
	fread(&chip->memory[start_address], end_address - start_address, 1, f);
	fclose(f);
}

void dump_memory_to_file(Chip8* chip, char* memory_file_name) {
	FILE* f = fopen(memory_file_name, "wb");
	if (!f) {
		exit(1);
	}
	fwrite(chip->memory, sizeof(uint8_t), CHIP8_MEMORY_SIZE, f);
	fclose(f);
}

void cycle(Chip8* chip) {
	if (!opcode_table[0]) {
		initialize();
	}

	chip->opcode = (chip->memory[chip->pc] << 8u) | chip->memory[chip->pc + 1];

	chip->pc += 2;

	opcode_table[(chip->opcode & 0xf000u) >> 12u](chip);

	if (chip->delay_timer > 0) {
		chip->delay_timer--;
	}

	if (chip->sound_timer > 0) {
		chip->sound_timer--;
	}
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
