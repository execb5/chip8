#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_PIXEL_COUNT (CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT)
#define CHIP8_KEYPAD_SIZE 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_REGISTER_COUNT 16
#define CHIP8_FONT_SET_START_ADDRESS 0x0050

typedef struct Chip8 {
	uint8_t registers[CHIP8_REGISTER_COUNT];
	uint8_t memory[CHIP8_MEMORY_SIZE];
	uint16_t index;
	uint16_t pc;
	uint16_t stack[CHIP8_STACK_SIZE];
	uint8_t sp;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint8_t keypad[CHIP8_KEYPAD_SIZE];
	uint8_t video[CHIP8_PIXEL_COUNT];
	uint16_t opcode;
} Chip8;

Chip8* create(void);
void load_rom(Chip8* chip, char* rom_name);
void dump_memory_to_file(Chip8* chip, char* memory_file_name);
void destroy(Chip8* chip);
uint8_t generate_random_byte(void);

#endif /* CHIP8_H */
