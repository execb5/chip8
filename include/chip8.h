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

Chip8* create();
Chip8* load_rom(Chip8* chip, char* rom_name);
void dump_memory_to_file(Chip8* chip, char* memory_file_name);

/**
 * @name 00E0
 * @brief Clear the display.
 *
 * @verbatim CLS @endverbatim
 *
 * @param chip State of the chip8 CPU.
 */
void op_00e0(Chip8* chip);

/**
 * @name 00EE
 * @brief Return from a subroutine.
 *
 * @verbatim RET @endverbatim
 *
 * The interpreter sets the program counter to the address at the top of the
 * stack, then subtracts 1 from the stack pointer.
 *
 * @param chip State of the chip8 CPU.
 */
void op_00ee(Chip8* chip);

/**
 * @name 1nnn
 * @brief Jump to location nnn.
 *
 * @verbatim JP addr @endverbatim
 *
 * The interpreter sets the program counter to nnn.
 *
 * @param chip State of the chip8 CPU.
 */
void op_1nnn(Chip8* chip);

/**
 * @name 2nnn
 * @brief Call subroutine at nnn.
 *
 * @verbatim CALL addr @endverbatim
 *
 * The interpreter increments the stack pointer, then puts the current PC on the
 * top of the stack. The PC is then set to nnn.
 *
 * @param chip State of the chip8 CPU.
 */
void op_2nnn(Chip8* chip);

/**
 * @name 3xkk
 * @brief Skip next instruction if Vx = kk.
 *
 * @verbatim SE Vx, byte @endverbatim
 *
 * The interpreter compares register Vx to kk, and if they are equal, increments
 * the program counter by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_3xkk(Chip8* chip);

/**
 * @name 4xkk
 * @brief Skip next instruction if Vx != kk.
 *
 * @verbatim SNE Vx, byte @endverbatim
 *
 * The interpreter compares register Vx to kk, and if they are not equal,
 * increments the program counter by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_4xkk(Chip8* chip);

/**
 *
 * @name 5xy0
 * @brief Skip next instruction if Vx = Vy.
 *
 * @verbatim SE Vx, Vy @endverbatim
 *
 * The interpreter compares register Vx to register Vy, and if they are equal,
 * increments the program counter by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_5xy0(Chip8* chip);

/**
 * @name 6xkk
 * @brief Set Vx = kk.
 *
 * @verbatim LD Vx, byte @endverbatim
 *
 * The interpreter puts the value kk into register Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_6xkk(Chip8* chip);

/**
 * @name 7xkk
 * @brief Set Vx = Vx + kk.
 *
 * @verbatim ADD Vx, byte @endverbatim
 *
 * Adds the value kk to the value of register Vx, then stores the result in Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_7xkk(Chip8* chip);

/**
 * @name 8xy0
 * @brief Set Vx = Vy.
 *
 * @verbatim LD Vx, Vy @endverbatim
 *
 * Stores the value of register Vy in register Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy0(Chip8* chip);

/**
 * @name 8xy1
 * @brief Set Vx = Vx OR Vy.
 *
 * @verbatim OR Vx, Vy @endverbatim
 *
 * Performs a bitwise OR on the values of Vx and Vy, then stores the result in
 * Vx. A bitwise OR compares the corrseponding bits from two values, and if
 * either bit is 1, then the same bit in the result is also 1. Otherwise, it is
 * 0.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy1(Chip8* chip);

/**
 * @name 8xy2
 * @brief Set Vx = Vx AND Vy.
 *
 * @verbatim AND Vx, Vy @endverbatim
 *
 * Performs a bitwise AND on the values of Vx and Vy, then stores the result in
 * Vx. A bitwise AND compares the corrseponding bits from two values, and if
 * both bits are 1, then the same bit in the result is also 1. Otherwise, it is
 * 0.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy2(Chip8* chip);

/**
 * @name 8xy3
 * @brief Set Vx = Vx XOR Vy.
 *
 * @verbatim XOR Vx, Vy @endverbatim
 *
 * Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the
 * result in Vx. An exclusive OR compares the corrseponding bits from two
 * values, and if the bits are not both the same, then the corresponding bit in
 * the result is set to 1. Otherwise, it is 0.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy3(Chip8* chip);

/**
 * @name 8xy4
 * @brief Set Vx = Vx + Vy, set VF = carry.
 *
 * @verbatim ADD Vx, Vy @endverbatim
 *
 * The values of Vx and Vy are added together. If the result is greater than 8
 * bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of
 * the result are kept, and stored in Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy4(Chip8* chip);

/**
 * @name 8xy5
 * @brief Set Vx = Vx - Vy, set VF = NOT borrow.
 *
 * @verbatim SUB Vx, Vy @endverbatim
 *
 * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx,
 * and the results stored in Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy5(Chip8* chip);

/**
 * @name 8xy6
 * @brief Set Vx = Vx SHR 1.
 *
 * @verbatim SHR Vx {, Vy} @endverbatim
 *
 * If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
 * Then Vx is divided by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy6(Chip8* chip);

/**
 * @name 8xy7
 * @brief Set Vx = Vy - Vx, set VF = NOT borrow.
 *
 * @verbatim SUBN Vx, Vy @endverbatim
 *
 * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy,
 * and the results stored in Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xy7(Chip8* chip);

/**
 * @name 8xyE
 * @brief Set Vx = Vx SHL 1.
 *
 * @verbatim SHL Vx {, Vy} @endverbatim
 *
 * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
 * Then Vx is multiplied by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_8xye(Chip8* chip);

/**
 * @name 9xy0
 * @brief Skip next instruction if Vx != Vy.
 *
 * @verbatim SNE Vx, Vy @endverbatim
 *
 * The values of Vx and Vy are compared, and if they are not equal, the program
 * counter is increased by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_9xy0(Chip8* chip);

/**
 * @name Annn
 * @brief Set I = nnn.
 *
 * @verbatim LD I, addr @endverbatim
 *
 * The value of register I is set to nnn.
 *
 * @param chip State of the chip8 CPU.
 */
void op_annn(Chip8* chip);

/**
 * @name Bnnn
 * @brief Jump to location nnn + V0.
 *
 * @verbatim JP V0, addr @endverbatim
 *
 * The program counter is set to nnn plus the value of V0.
 * @param chip State of the chip8 CPU.
 */
void op_bnnn(Chip8* chip);

/**
 * @name Cxkk
 * @brief Set Vx = random byte AND kk.
 *
 * @verbatim RND Vx, byte @endverbatim
 *
 * The interpreter generates a random number from 0 to 255, which is then ANDed
 * with the value kk. The results are stored in Vx.
 *
 * @param chip State of the chip8 CPU.
 * @param byte_generator_function Function to generate a random number between 0
 * and 255.
 */
void op_cxkk(Chip8* chip, uint8_t (*byte_generator_function)());

/**
 * @name Dxyn
 * @brief Display n-byte sprite starting at memory location I at (Vx, Vy), set
 * VF = collision.
 *
 * @verbatim DRW Vx, Vy, nibble @endverbatim
 *
 * The interpreter reads n bytes from memory, starting at the address stored in
 * I. These bytes are then displayed as sprites on screen at coordinates (Vx,
 * Vy). Sprites are XORed onto the existing screen. If this causes any pixels to
 * be erased, VF is set to 1, otherwise it is set to 0. If the sprite is
 * positioned so part of it is outside the coordinates of the display, it wraps
 * around to the opposite side of the screen.
 *
 * @param chip State of the chip8 CPU.
 */
void op_dxyn(Chip8* chip);
void destroy(Chip8* chip);
uint8_t generate_random_byte(void);

#endif /* CHIP8_H */
