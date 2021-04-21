#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef struct Chip8 {
	uint8_t registers[16];
	uint8_t memory[4096];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[16];
	uint8_t sp;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t keypad[16];
	uint32_t video[64 * 32];
	uint16_t opcode;
} Chip8;

Chip8* start(char* rom_name);
void dump_memory_to_file(Chip8* chip, char* memory_file_name);

#endif /* CHIP8_H */

