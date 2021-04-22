#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

extern int screen_width;
extern int screen_height;

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_PIXEL_COUNT (CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT)
#define CHIP8_KEYPAD_SIZE 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_REGISTER_COUNT 16

typedef struct Chip8 {
	uint8_t registers[CHIP8_REGISTER_COUNT];
	uint8_t memory[CHIP8_MEMORY_SIZE];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[CHIP8_STACK_SIZE];
	uint8_t sp;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t keypad[CHIP8_KEYPAD_SIZE];
	uint32_t video[CHIP8_PIXEL_COUNT];
	uint16_t opcode;
} Chip8;

Chip8* start(char* rom_name);
void dump_memory_to_file(Chip8* chip, char* memory_file_name);
void op_00e0(Chip8* chip);
void op_00ee(Chip8* chip);

#endif /* CHIP8_H */
