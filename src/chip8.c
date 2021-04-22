#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

static uint16_t start_address = 0x0200;
static uint16_t end_address = 0x0fff;
static uint16_t font_set_start_address = 0x0050;

static uint8_t FONT_SET_SIZE = 80;
uint8_t font_set[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static int rand_number() {
	return rand() % 256;
}

Chip8* create(char* rom_name) {
	Chip8 *a = calloc(1, sizeof(Chip8));
	if (!a) {
		exit(2);
	}
	FILE *f = fopen(rom_name, "r");
	if (!f) {
		exit(1);
	}
	fread(&a->memory[start_address], end_address - start_address, 1, f);
	fclose(f);
	memcpy(&a->memory[font_set_start_address], font_set, FONT_SET_SIZE);
	srand(time(NULL));
	return a;
}

void dump_memory_to_file(Chip8* chip, char* memory_file_name) {
	FILE *f = fopen(memory_file_name, "wb");
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

void destroy(Chip8* chip) {
	free(chip);
}
