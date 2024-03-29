#include <string.h>
#include "../inc/instructions.h"

void op_00e0(Chip8* chip) {
	memset(chip->video, 0, sizeof(chip->video));
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

void op_3xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	if (chip->registers[vx] == kk) {
		chip->pc += 2;
	}
}

void op_4xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	if (chip->registers[vx] != kk) {
		chip->pc += 2;
	}
}

void op_5xy0(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vx] == chip->registers[vy]) {
		chip->pc += 2;
	}
}

void op_6xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	chip->registers[vx] = kk;
}

void op_7xkk(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = (chip->opcode & 0x00ffu);

	chip->registers[vx] += kk;
}

void op_8xy0(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] = chip->registers[vy];
}

void op_8xy1(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] |= chip->registers[vy];
}

void op_8xy2(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] &= chip->registers[vy];
}

void op_8xy3(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	chip->registers[vx] ^= chip->registers[vy];
}

void op_8xy4(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	uint16_t sum = chip->registers[vx] + chip->registers[vy];

	if (sum > 0xffu) {
		chip->registers[0xf] = 1;
	} else {
		chip->registers[0xf] = 0;
	}

	chip->registers[vx] = sum & 0x00ffu;
}

void op_8xy5(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vx] > chip->registers[vy]) {
		chip->registers[0xf] = 1;
	} else {
		chip->registers[0xf] = 0;
	}

	chip->registers[vx] -= chip->registers[vy];
}

void op_8xy6(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->registers[0xf] = (chip->registers[vx] & 0x0001u);

	chip->registers[vx] >>= 1;
}

void op_8xy7(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vy] > chip->registers[vx]) {
		chip->registers[0xf] = 1;
	} else {
		chip->registers[0xf] = 0;
	}

	chip->registers[vx] = chip->registers[vy] - chip->registers[vx];
}

void op_8xye(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	uint8_t most_significant_bit_of_vx = chip->registers[vx] >> 7u;
	chip->registers[0xf] = most_significant_bit_of_vx;

	chip->registers[vx] <<= 1;
}

void op_9xy0(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;

	if (chip->registers[vx] != chip->registers[vy]) {
		chip->pc += 2;
	}
}

void op_annn(Chip8* chip) {
	chip->index = chip->opcode & 0x0fffu;
}

void op_bnnn(Chip8* chip) {
	chip->pc = chip->registers[0x0] + (chip->opcode & 0x0fffu);
}

void op_cxkk(Chip8* chip, uint8_t (*byte_generator_function)()) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t kk = chip->opcode & 0x00ffu;

	uint8_t random_byte = (*byte_generator_function)();

	chip->registers[vx] = random_byte & kk;
}

void op_dxyn(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vy = (chip->opcode & 0x00f0u) >> 4u;
	uint8_t n = chip->opcode & 0x000fu;

	uint8_t x_start = chip->registers[vx] % CHIP8_SCREEN_WIDTH;
	uint8_t y_start = chip->registers[vy] % CHIP8_SCREEN_HEIGHT;

	chip->registers[0xf] = 0x00;

	for (uint8_t row = 0; row < n; row++) {
		uint8_t sprite_byte = chip->memory[chip->index + row];

		for (uint8_t column = 0; column < 8; column++) {
			uint8_t sprite_pixel = sprite_byte & (0x80u >> column);
			uint32_t* screen_pixel = &chip->video[(y_start + row) * CHIP8_SCREEN_WIDTH + (x_start + column)];

			if (sprite_pixel) {
				if (*screen_pixel == 0xffffffff) {
					chip->registers[0xf] = 0x01;
				}

				*screen_pixel ^= 0xffffffff;
			}
		}
	}
}

void op_ex9e(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	uint8_t key = chip->registers[vx];

	if (chip->keypad[key]) {
		chip->pc += 2;
	}
}

void op_exa1(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	uint8_t key = chip->registers[vx];

	if (!chip->keypad[key]) {
		chip->pc += 2;
	}
}

void op_fx07(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->registers[vx] = chip->delay_timer;
}

void op_fx0a(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	if (chip->keypad[0x0]) {
		chip->registers[vx] = 0x0;
	} else if (chip->keypad[0x1]) {
		chip->registers[vx] = 0x1;
	} else if (chip->keypad[0x2]) {
		chip->registers[vx] = 0x2;
	} else if (chip->keypad[0x3]) {
		chip->registers[vx] = 0x3;
	} else if (chip->keypad[0x4]) {
		chip->registers[vx] = 0x4;
	} else if (chip->keypad[0x5]) {
		chip->registers[vx] = 0x5;
	} else if (chip->keypad[0x6]) {
		chip->registers[vx] = 0x6;
	} else if (chip->keypad[0x7]) {
		chip->registers[vx] = 0x7;
	} else if (chip->keypad[0x8]) {
		chip->registers[vx] = 0x8;
	} else if (chip->keypad[0x9]) {
		chip->registers[vx] = 0x9;
	} else if (chip->keypad[0xa]) {
		chip->registers[vx] = 0xa;
	} else if (chip->keypad[0xb]) {
		chip->registers[vx] = 0xb;
	} else if (chip->keypad[0xc]) {
		chip->registers[vx] = 0xc;
	} else if (chip->keypad[0xd]) {
		chip->registers[vx] = 0xd;
	} else if (chip->keypad[0xe]) {
		chip->registers[vx] = 0xe;
	} else if (chip->keypad[0xf]) {
		chip->registers[vx] = 0xf;
	} else {
		chip->opcode -= 2;
	}
}

void op_fx15(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->delay_timer = chip->registers[vx];
}

void op_fx18(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->sound_timer = chip->registers[vx];
}

void op_fx1e(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->index += chip->registers[vx];
}

void op_fx29(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	chip->index = CHIP8_FONT_SET_START_ADDRESS + (5 * chip->registers[vx]);
}

void op_fx33(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;
	uint8_t vx_value = chip->registers[vx];

	chip->memory[chip->index + 2] = vx_value % 10;
	vx_value /= 10;

	chip->memory[chip->index + 1] = vx_value % 10;
	vx_value /= 10;

	chip->memory[chip->index] = vx_value % 10;
}

void op_fx55(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	for (uint8_t i = 0; i <= vx; i++) {
		chip->memory[chip->index + i] = chip->registers[i];
	}
}

void op_fx65(Chip8* chip) {
	uint8_t vx = (chip->opcode & 0x0f00u) >> 8u;

	for (uint8_t i = 0; i <= vx; i++) {
		chip->registers[i] = chip->memory[chip->index + i];
	}
}
