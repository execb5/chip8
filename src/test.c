#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "chip8.h"

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
	int previous_sp_value = a.sp - 1;
	a.stack[previous_sp_value] = 0x10;

	op_00ee(&a);

	assert_int_equal(a.pc, a.stack[a.sp]);
}

static void test_op_00ee_should_decrease_sp_by_one() {
	Chip8 a;
	a.sp = 0x05;
	int previous_sp_value = a.sp - 1;

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

int main(void) {
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
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
