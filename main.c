#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define START_ADDRESS 0x200
#define END_ADDRESS 0xfff
#define FONT_SET_START_ADDRESS 0x50
#define FONT_SET_SIZE 80

uint8_t font_set[FONT_SET_SIZE] =
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

int main() {
    Chip8 *a = malloc(sizeof(Chip8));
    FILE *f = fopen("/Users/mnunes/repos/chip8/pong.ch8", "r");
    if (!f) {
        return 1;
    }
    fread(&a->memory[START_ADDRESS], sizeof(END_ADDRESS - START_ADDRESS), 1, f);
    fclose(f);
    memcpy(&a->memory[FONT_SET_START_ADDRESS], font_set, FONT_SET_SIZE);
    return 0;
}
