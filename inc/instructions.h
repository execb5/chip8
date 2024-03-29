#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "chip8.h"

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

/**
 * @name Ex9E
 * @brief Skip next instruction if key with the value of Vx is pressed.
 *
 * @verbatim SKP Vx @endverbatim
 *
 * Checks the keyboard, and if the key corresponding to the value of Vx is
 * currently in the down position, PC is increased by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_ex9e(Chip8* chip);

/**
 * @name ExA1
 * @brief Skip next instruction if key with the value of Vx is not pressed.
 *
 * @verbatim SKNP Vx @endverbatim
 *
 * Checks the keyboard, and if the key corresponding to the value of Vx is
 * currently in the up position, PC is increased by 2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_exa1(Chip8* chip);

/**
 * @name Fx07
 * @brief Set Vx = delay timer value.
 *
 * @verbatim LD Vx, DT @endverbatim
 *
 * The value of DT is placed into Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx07(Chip8* chip);

/**
 * @name Fx0A
 * @brief Wait for a key press, store the value of the key in Vx.
 *
 * @verbatim LD Vx, K @endverbatim
 *
 * All execution stops until a key is pressed, then the value of that key is
 * stored in Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx0a(Chip8* chip);

/**
 * @name Fx15
 * @brief Set delay timer = Vx.
 *
 * @verbatim LD DT, Vx @endverbatim
 *
 * DT is set equal to the value of Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx15(Chip8* chip);

/**
 * @name Fx18
 * @brief Set sound timer = Vx.
 *
 * @verbatim LD ST, Vx @endverbatim
 *
 * ST is set equal to the value of Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx18(Chip8* chip);

/**
 * @name Fx1E
 * @brief Set I = I + Vx.
 *
 * @verbatim ADD I, Vx @endverbatim
 *
 * The values of I and Vx are added, and the results are stored in I.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx1e(Chip8* chip);

/**
 * @name Fx29
 * @brief Set I = location of sprite for digit Vx.
 *
 * @verbatim LD F, Vx @endverbatim
 *
 * The value of I is set to the location for the hexadecimal sprite
 * corresponding to the value of Vx. See section 2.4, Display, for more
 * information on the Chip-8 hexadecimal font.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx29(Chip8* chip);

/**
 * @name Fx33
 * @brief Store BCD representation of Vx in memory locations I, I+1, and I+2.
 *
 * @verbatim LD B, Vx @endverbatim
 *
 * The interpreter takes the decimal value of Vx, and places the hundreds digit
 * in memory at location in I, the tens digit at location I+1, and the ones
 * digit at location I+2.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx33(Chip8* chip);

/**
 * @name Fx55
 * @brief Store registers V0 through Vx in memory starting at location I.
 *
 * @verbatim LD [I], Vx @endverbatim
 *
 * The interpreter copies the values of registers V0 through Vx into memory,
 * starting at the address in I.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx55(Chip8* chip);

/**
 * @name Fx65
 * @brief Read registers V0 through Vx from memory starting at location I.
 *
 * @verbatim LD Vx, [I] @endverbatim
 *
 * The interpreter reads values from memory starting at location I into
 * registers V0 through Vx.
 *
 * @param chip State of the chip8 CPU.
 */
void op_fx65(Chip8* chip);

#endif /* INSTRUCTIONS_H */
