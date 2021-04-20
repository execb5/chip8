#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Chip8
{
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
    Chip8* a = malloc(sizeof(Chip8));
    printf("Hello, World!\n");
    return 0;
}
