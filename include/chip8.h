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
	uint8_t video[CHIP8_PIXEL_COUNT];
	uint16_t opcode;
} Chip8;

Chip8* create(char* rom_name);
void dump_memory_to_file(Chip8* chip, char* memory_file_name);
void op_00e0(Chip8* chip); // CLS
void op_00ee(Chip8* chip); // RET
void op_1nnn(Chip8* chip); // JP addr
void op_2nnn(Chip8* chip); // CALL addr
void op_3xkk(Chip8* chip); // SE Vx, byte
void op_4xkk(Chip8* chip); // SNE Vx, byte
void op_5xy0(Chip8* chip); // SE Vx, Vy
void op_6xkk(Chip8* chip); // LD Vx, byte
void op_7xkk(Chip8* chip); // ADD Vx, byte
void op_8xy0(Chip8* chip); // LD Vx, Vy
void op_8xy1(Chip8* chip); // OR Vx, Vy
void destroy(Chip8* chip);

#endif /* CHIP8_H */
