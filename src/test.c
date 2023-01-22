#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "../include/chip8.h"

static uint32_t next = 1;

static void my_cute_srand(uint32_t seed) {
	next = seed;
}

static uint8_t my_cute_rand(void) {
	next = next * 1103515245 + 12345;
	return (uint8_t) (next / 65536) % 32768;
}

static void test_op_00e0_should_fill_memory_with_zeroes() {
	Chip8 a;
	memset(a.video, 0x01, CHIP8_PIXEL_COUNT);

	op_00e0(&a);

	for (int i = 0; i < CHIP8_PIXEL_COUNT; i++) {
		assert_int_equal(a.video[i], 0);
	}
}

static void test_op_00ee_should_set_the_pc_to_address_on_top_of_stack() {
	Chip8 a;
	a.sp = 0x05;
	uint8_t previous_sp_value = a.sp - 1;
	a.stack[previous_sp_value] = 0x10;

	op_00ee(&a);

	assert_int_equal(a.pc, a.stack[a.sp]);
}

static void test_op_00ee_should_decrease_sp_by_one() {
	Chip8 a;
	a.sp = 0x05;
	uint8_t previous_sp_value = a.sp - 1;

	op_00ee(&a);

	assert_int_equal(a.sp, previous_sp_value);
}

static void test_op_1nnn_should_set_the_pc_to_nnn() {
	Chip8 a;
	a.opcode = 0x9006;
	uint16_t address = a.opcode & 0x0fffu;

	op_1nnn(&a);

	assert_int_equal(a.pc, address);
}

static void test_op_2nnn_should_increment_the_sp() {
	Chip8 a;
	uint8_t sp = 0x0au;
	a.sp = sp;
	a.stack[a.sp] = 0;
	a.pc = 0x0020;
	a.opcode = 0x9006;

	op_2nnn(&a);

	assert_int_equal(a.sp, sp + 0x01u);
}

static void test_op_2nnn_should_put_the_current_pc_on_the_top_of_the_stack() {
	Chip8 a;
	uint8_t sp = 0x0au;
	a.sp = sp;
	a.stack[a.sp] = 0;
	a.pc = 0x0020;
	a.opcode = 0x9006;

	op_2nnn(&a);

	assert_int_equal(a.stack[sp], 0x0020);
}

static void test_op_2nnn_should_set_the_pc_to_nnn() {
	Chip8 a;
	uint8_t sp = 0x0au;
	a.sp = sp;
	a.stack[a.sp] = 0;
	a.pc = 0x0020;
	a.opcode = 0x9006;
	uint16_t address = a.opcode & 0x0fffu;

	op_2nnn(&a);

	assert_int_equal(a.pc, address);
}

static void test_op_3xkk_should_increment_pc_by_two_if_vx_and_kk_are_equal() {
	Chip8 a;
	a.opcode = 0x0202;
	a.registers[0x02] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_3xkk(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_3xkk_should_maintain_pc_if_vx_and_kk_are_different() {
	Chip8 a;
	a.opcode = 0x0302;
	a.registers[0x03] = 0x03;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_3xkk(&a);

	assert_int_equal(a.pc, pc);
}

static void test_op_3xkk_should_increase_pc_by_two_if_leftmost_byte_is_different_than_k() {
	Chip8 a;
	a.opcode = 0x1202;
	a.registers[0x02] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_3xkk(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_4xkk_should_increment_pc_by_two_if_vx_and_kk_are_different() {
	Chip8 a;
	a.opcode = 0x0203;
	a.registers[0x02] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_4xkk(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_4xkk_should_maintain_pc_if_vx_and_kk_are_equal() {
	Chip8 a;
	a.opcode = 0x0303;
	a.registers[0x03] = 0x03;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_4xkk(&a);

	assert_int_equal(a.pc, pc);
}

static void test_op_4xkk_should_increase_pc_by_two_if_leftmost_byte_is_different_than_k() {
	Chip8 a;
	a.opcode = 0x1203;
	a.registers[0x02] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_4xkk(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_5xy0_should_increment_pc_by_two_if_vx_and_vy_are_equal() {
	Chip8 a;
	a.opcode = 0x0230;
	a.registers[0x02] = 0x02;
	a.registers[0x03] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_5xy0(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_5xy0_should_maintain_pc_if_vx_and_vy_are_different() {
	Chip8 a;
	a.opcode = 0x0230;
	a.registers[0x02] = 0x02;
	a.registers[0x03] = 0x03;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_5xy0(&a);

	assert_int_equal(a.pc, pc);
}

static void test_op_5xy0_should_increase_pc_by_two_if_leftmost_byte_is_different_than_y() {
	Chip8 a;
	a.opcode = 0x1230;
	a.registers[0x02] = 0x02;
	a.registers[0x03] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_5xy0(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_5xy0_should_increase_pc_by_two_if_rightmost_byte_is_different_than_x() {
	Chip8 a;
	a.opcode = 0x0231;
	a.registers[0x02] = 0x02;
	a.registers[0x03] = 0x02;
	uint16_t pc = 0x0050u;
	a.pc = pc;

	op_5xy0(&a);

	assert_int_equal(a.pc, pc + 2);
}

static void test_op_6xkk_should_save_kk_in_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t kk = 0x31;
	a.opcode = (vx << 8u) + kk;
	a.registers[vx] = 0x00;

	op_6xkk(&a);

	assert_int_equal(a.registers[vx], kk);
}

static void test_op_7xkk_should_add_kk_to_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t kk = 0x31;
	uint8_t previous_value = 0x30;
	a.opcode = (vx << 8u) + kk;
	a.registers[vx] = previous_value;

	op_7xkk(&a);

	assert_int_equal(a.registers[vx], previous_value + kk);
}

static void test_op_8xy0_should_save_vy_in_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vy_value = 0x0a;
	a.registers[vy] = vy_value;
	a.registers[vx] = 0x00;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy0(&a);

	assert_int_equal(a.registers[vx], vy_value);
}

static void test_op_8xy1_should_set_vx_or_vy_to_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xf0;
	uint8_t vy_value = 0x0f;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy1(&a);

	assert_int_equal(a.registers[vx], vx_value | vy_value);
}

static void test_op_8xy2_should_set_vx_and_vy_to_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xf0;
	uint8_t vy_value = 0x0f;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy2(&a);

	assert_int_equal(a.registers[vx], vx_value & vy_value);
}

static void test_op_8xy3_should_set_vx_xor_vy_to_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xf0;
	uint8_t vy_value = 0x0f;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy3(&a);

	assert_int_equal(a.registers[vx], vx_value ^ vy_value);
}

static void test_op_8xy4_should_set_vx_plus_vy_to_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xf0;
	uint8_t vy_value = 0x0f;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy4(&a);

	assert_int_equal(a.registers[vx], vx_value + vy_value);
}

static void test_op_8xy4_should_set_overflow_flag_on_sums_bigger_than_size() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xff;
	uint8_t vy_value = 0x01;
	uint8_t sum = vx_value + vy_value;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy4(&a);

	assert_int_equal(a.registers[vx], sum & 0xffu);
	assert_int_equal(a.registers[0xf], 1);
}

static void test_op_8xy5_should_set_vx_minus_vy_to_register_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xff;
	uint8_t vy_value = 0x01;
	uint8_t sub = vx_value - vy_value;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy5(&a);

	assert_int_equal(a.registers[vx], sub);
}

static void test_op_8xy5_should_set_1_to_vf_if_vx_greater_than_vy() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xff;
	uint8_t vy_value = 0x01;
	uint8_t sub = vx_value - vy_value;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy5(&a);

	assert_int_equal(a.registers[vx], sub);
	assert_int_equal(a.registers[0xf], 1);
}

static void test_op_8xy5_should_set_0_to_vf_if_vx_lesser_than_vy() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0x01;
	uint8_t vy_value = 0xff;
	uint8_t sub = vx_value - vy_value;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy5(&a);

	assert_int_equal(a.registers[vx], sub & 0xffu);
	assert_int_equal(a.registers[0xf], 0);
}

static void test_op_8xy6_should_set_the_least_significant_bit_of_vx_to_vf() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x0b;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);

	uint8_t least_significant_bit = (vx_value & 0x1u);

	op_8xy6(&a);

	assert_int_equal(a.registers[0xf], least_significant_bit);
}

static void test_op_8xy6_should_divide_vx_by_2() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x0b;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);

	op_8xy6(&a);

	assert_int_equal(a.registers[vx], (vx_value >> 1));
}

static void test_op_8xy7_should_set_vx_to_vy_minus_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xff;
	uint8_t vy_value = 0x01;
	uint8_t sub = vy_value - vx_value;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy7(&a);

	assert_int_equal(a.registers[vx], sub);
}

static void test_op_8xy7_should_set_vf_to_1_if_vy_greater_than_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0x01;
	uint8_t vy_value = 0xff;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy7(&a);

	assert_int_equal(a.registers[0xf], 1);
}

static void test_op_8xy7_should_set_vf_to_0_if_vy_lesser_than_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0xff;
	uint8_t vy_value = 0x01;
	a.registers[vy] = vy_value;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_8xy7(&a);

	assert_int_equal(a.registers[0xf], 0);
}

static void test_op_8xye_should_multiply_vx_by_two() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x04;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);

	op_8xye(&a);

	assert_int_equal(a.registers[vx], (vx_value << 1));
}

static void test_op_8xye_should_set_vf_to_1_if_most_significant_bit_is_1() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x82;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);

	op_8xye(&a);

	assert_int_equal(a.registers[0xf], 1);
}

static void test_op_8xye_should_set_vf_to_0_if_most_significant_bit_is_0() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x52;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);

	op_8xye(&a);

	assert_int_equal(a.registers[0xf], 0);
}

static void test_op_9xy0_should_increment_pc_by_two_if_vx_is_different_than_vy() {
	Chip8 a;
	a.pc = 0;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0x00;
	uint8_t vy_value = 0x10;
	a.registers[vx] = vx_value;
	a.registers[vy] = vy_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_9xy0(&a);

	assert_int_equal(a.pc, 2);
}

static void test_op_9xy0_should_maintain_pc_if_vx_is_equal_to_vy() {
	Chip8 a;
	a.pc = 0;
	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0x10;
	uint8_t vy_value = 0x10;
	a.registers[vx] = vx_value;
	a.registers[vy] = vy_value;
	a.opcode = (vx << 8u) + (vy << 4u);

	op_9xy0(&a);

	assert_int_equal(a.pc, 0);
}

static void test_op_annn_should_set_nnn_to_index() {
	Chip8 a;
	a.opcode = 0xa123;

	op_annn(&a);

	assert_int_equal(a.index, 0x0123);
}

static void test_op_bnnn_should_set_pc_to_nnn_plus_v0() {
	Chip8 a;
	a.opcode = 0xb123;
	a.registers[0] = 0x50;

	op_bnnn(&a);

	assert_int_equal(a.pc, 0x0173);
}

static void test_op_cxkk_should_set_vx_to_kk_and_a_random_number_between_0_and_255() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a.registers[vx] = vx_value;
	uint8_t kk = 0xff;
	a.opcode = (vx << 8u) + kk;

	op_cxkk(&a, my_cute_rand);

	assert_in_range(a.registers[vx], 0, 255);
}

static void test_op_cxkk_should_set_vx_to_zero_if_kk_is_zero() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a.registers[vx] = vx_value;
	uint8_t kk = 0x00;
	a.opcode = (vx << 8u) + kk;

	op_cxkk(&a, my_cute_rand);

	assert_int_equal(a.registers[vx], 0);
}

static void test_op_cxkk_should_set_vx_to_kk_and_a_random_number() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a.registers[vx] = vx_value;
	uint8_t kk = 0xff;
	a.opcode = (vx << 8u) + kk;

	my_cute_srand(0xfaaffaaf);

	op_cxkk(&a, my_cute_rand);

	assert_int_equal(a.registers[vx], 0xa9);
}

static void test_op_dxyn_draws_sprite_in_position_defined_by_vx_and_vy() {
	Chip8 a;
	a.index = 0x00ff;
	uint8_t n = 0x08;

	memset(a.video, 0x00, CHIP8_PIXEL_COUNT);

	// sprite is a square 8x8
	for (uint8_t row = 0x00; row < n; ++row) {
		a.memory[a.index + row] = 0xff;
	}

	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0x0a;
	uint8_t vy_value = 0x05;
	a.registers[vx] = vx_value;
	a.registers[vy] = vy_value;
	a.opcode = 0xd000 + (vx << 8u) + (vy << 4u) + n;

	op_dxyn(&a);

	for (uint8_t row = 0; row < n; ++row) {
		for (int column = 0; column < 8; ++column) {
			assert_int_equal(a.video[(vy_value + row) * CHIP8_SCREEN_WIDTH + (vx_value + column)], 0xff);
		}
	}
}

static void test_op_dxyn_sets_vf_to_1_if_there_is_sprite_collision() {
	Chip8 a;
	a.index = 0x00ff;
	uint8_t n = 0x08;

	memset(a.video, 0xff, CHIP8_PIXEL_COUNT);

	// sprite is a square 8x8
	for (uint8_t row = 0x00; row < n; ++row) {
		a.memory[a.index + row] = 0xff;
	}

	uint8_t vx = 0x02;
	uint8_t vy = 0x03;
	uint8_t vx_value = 0x0a;
	uint8_t vy_value = 0x05;
	a.registers[vx] = vx_value;
	a.registers[vy] = vy_value;
	a.opcode = 0xd000 + (vx << 8u) + (vy << 4u) + n;

	op_dxyn(&a);

	assert_int_equal(a.registers[0xf], 0x01);
}

static void test_op_ex9e_should_increment_pc_if_key_with_the_value_of_vx_is_pressed() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x0a;
	a.keypad[vx_value] = 0xff;
	a.opcode = (vx << 8u) + 0xe09e;
	a.pc = 0x0000;

	op_ex9e(&a);

	assert_int_equal(a.pc, 0x0002);
}

static void test_op_ex9e_should_not_increment_pc_if_key_with_the_value_of_vx_is_not_pressed() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x0a;
	a.keypad[vx_value] = 0x00;
	a.opcode = (vx << 8u) + 0xe09e;
	a.pc = 0x0000;

	op_ex9e(&a);

	assert_int_equal(a.pc, 0x0000);
}

static void test_op_exa1_should_not_increment_pc_if_key_with_the_value_of_vx_is_pressed() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x0a;
	a.keypad[vx_value] = 0xff;
	a.opcode = (vx << 8u) + 0xe09e;
	a.pc = 0x0000;

	op_exa1(&a);

	assert_int_equal(a.pc, 0x0000);
}

static void test_op_exa1_should_increment_pc_if_key_with_the_value_of_vx_is_not_pressed() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x0a;
	a.keypad[vx_value] = 0x00;
	a.opcode = (vx << 8u) + 0xe09e;
	a.pc = 0x0000;

	op_exa1(&a);

	assert_int_equal(a.pc, 0x0002);
}

static void test_op_fx07_should_set_vx_to_the_value_of_delay_timer() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);
	a.delayTimer = 0x15;

	op_fx07(&a);

	assert_int_equal(a.registers[vx], a.delayTimer);
}

static void test_op_fx0a_should_set_vx_to_the_value_of_the_key_pressed() {
	my_cute_srand(rand());
	int i = my_cute_rand() % 16;
	Chip8* a = create();
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a->registers[vx] = vx_value;
	a->opcode = (vx << 8u);
	a->keypad[i] = 1;

	op_fx0a(a);

	assert_int_equal(a->registers[vx], i);

	destroy(a);
}

static void test_op_fx0a_should_increment_pc_if_no_key_is_pressed() {
	Chip8* a = create();
	uint8_t vx = 0x02;
	a->opcode = (vx << 8u);
	a->pc = 0x0020;

	op_fx0a(a);

	assert_int_equal(a->pc, 0x0020);

	destroy(a);
}

static void test_op_fx15_should_set_delay_timer_to_the_value_of_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);
	a.delayTimer = 0x15;

	op_fx15(&a);

	assert_int_equal(a.delayTimer, vx_value);
}

static void test_op_fx18_should_set_sound_timer_to_the_value_of_vx() {
	Chip8 a;
	uint8_t vx = 0x02;
	uint8_t vx_value = 0x10;
	a.registers[vx] = vx_value;
	a.opcode = (vx << 8u);
	a.soundTimer = 0x15;

	op_fx18(&a);

	assert_int_equal(a.soundTimer, vx_value);
}

int main(void) {
	srand(time(NULL));
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_op_00e0_should_fill_memory_with_zeroes),
		cmocka_unit_test(test_op_00ee_should_set_the_pc_to_address_on_top_of_stack),
		cmocka_unit_test(test_op_00ee_should_decrease_sp_by_one),
		cmocka_unit_test(test_op_1nnn_should_set_the_pc_to_nnn),
		cmocka_unit_test(test_op_2nnn_should_increment_the_sp),
		cmocka_unit_test(test_op_2nnn_should_put_the_current_pc_on_the_top_of_the_stack),
		cmocka_unit_test(test_op_2nnn_should_set_the_pc_to_nnn),
		cmocka_unit_test(test_op_3xkk_should_increment_pc_by_two_if_vx_and_kk_are_equal),
		cmocka_unit_test(test_op_3xkk_should_maintain_pc_if_vx_and_kk_are_different),
		cmocka_unit_test(test_op_3xkk_should_increase_pc_by_two_if_leftmost_byte_is_different_than_k),
		cmocka_unit_test(test_op_4xkk_should_increment_pc_by_two_if_vx_and_kk_are_different),
		cmocka_unit_test(test_op_4xkk_should_maintain_pc_if_vx_and_kk_are_equal),
		cmocka_unit_test(test_op_4xkk_should_increase_pc_by_two_if_leftmost_byte_is_different_than_k),
		cmocka_unit_test(test_op_5xy0_should_increment_pc_by_two_if_vx_and_vy_are_equal),
		cmocka_unit_test(test_op_5xy0_should_maintain_pc_if_vx_and_vy_are_different),
		cmocka_unit_test(test_op_5xy0_should_increase_pc_by_two_if_leftmost_byte_is_different_than_y),
		cmocka_unit_test(test_op_5xy0_should_increase_pc_by_two_if_rightmost_byte_is_different_than_x),
		cmocka_unit_test(test_op_6xkk_should_save_kk_in_register_vx),
		cmocka_unit_test(test_op_7xkk_should_add_kk_to_register_vx),
		cmocka_unit_test(test_op_8xy0_should_save_vy_in_vx),
		cmocka_unit_test(test_op_8xy1_should_set_vx_or_vy_to_register_vx),
		cmocka_unit_test(test_op_8xy2_should_set_vx_and_vy_to_register_vx),
		cmocka_unit_test(test_op_8xy3_should_set_vx_xor_vy_to_register_vx),
		cmocka_unit_test(test_op_8xy4_should_set_vx_plus_vy_to_register_vx),
		cmocka_unit_test(test_op_8xy4_should_set_overflow_flag_on_sums_bigger_than_size),
		cmocka_unit_test(test_op_8xy5_should_set_vx_minus_vy_to_register_vx),
		cmocka_unit_test(test_op_8xy5_should_set_1_to_vf_if_vx_greater_than_vy),
		cmocka_unit_test(test_op_8xy5_should_set_0_to_vf_if_vx_lesser_than_vy),
		cmocka_unit_test(test_op_8xy6_should_set_the_least_significant_bit_of_vx_to_vf),
		cmocka_unit_test(test_op_8xy6_should_divide_vx_by_2),
		cmocka_unit_test(test_op_8xy7_should_set_vx_to_vy_minus_vx),
		cmocka_unit_test(test_op_8xy7_should_set_vf_to_1_if_vy_greater_than_vx),
		cmocka_unit_test(test_op_8xy7_should_set_vf_to_0_if_vy_lesser_than_vx),
		cmocka_unit_test(test_op_8xye_should_multiply_vx_by_two),
		cmocka_unit_test(test_op_8xye_should_set_vf_to_1_if_most_significant_bit_is_1),
		cmocka_unit_test(test_op_8xye_should_set_vf_to_0_if_most_significant_bit_is_0),
		cmocka_unit_test(test_op_9xy0_should_increment_pc_by_two_if_vx_is_different_than_vy),
		cmocka_unit_test(test_op_9xy0_should_maintain_pc_if_vx_is_equal_to_vy),
		cmocka_unit_test(test_op_annn_should_set_nnn_to_index),
		cmocka_unit_test(test_op_bnnn_should_set_pc_to_nnn_plus_v0),
		cmocka_unit_test(test_op_cxkk_should_set_vx_to_kk_and_a_random_number_between_0_and_255),
		cmocka_unit_test(test_op_cxkk_should_set_vx_to_zero_if_kk_is_zero),
		cmocka_unit_test(test_op_cxkk_should_set_vx_to_kk_and_a_random_number),
		cmocka_unit_test(test_op_dxyn_draws_sprite_in_position_defined_by_vx_and_vy),
		cmocka_unit_test(test_op_dxyn_sets_vf_to_1_if_there_is_sprite_collision),
		cmocka_unit_test(test_op_ex9e_should_increment_pc_if_key_with_the_value_of_vx_is_pressed),
		cmocka_unit_test(test_op_ex9e_should_not_increment_pc_if_key_with_the_value_of_vx_is_not_pressed),
		cmocka_unit_test(test_op_exa1_should_not_increment_pc_if_key_with_the_value_of_vx_is_pressed),
		cmocka_unit_test(test_op_exa1_should_increment_pc_if_key_with_the_value_of_vx_is_not_pressed),
		cmocka_unit_test(test_op_fx07_should_set_vx_to_the_value_of_delay_timer),
		cmocka_unit_test(test_op_fx0a_should_set_vx_to_the_value_of_the_key_pressed),
		cmocka_unit_test(test_op_fx0a_should_increment_pc_if_no_key_is_pressed),
		cmocka_unit_test(test_op_fx15_should_set_delay_timer_to_the_value_of_vx),
		cmocka_unit_test(test_op_fx18_should_set_sound_timer_to_the_value_of_vx),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
