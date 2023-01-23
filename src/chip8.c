#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/chip8.h"

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

Chip8* create(void) {
	Chip8* a = calloc(1, sizeof(Chip8));

	if (!a) {
		exit(2);
	}

	memcpy(&a->memory[CHIP8_FONT_SET_START_ADDRESS], font_set, font_set_size);

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
