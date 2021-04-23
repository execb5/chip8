#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "chip8.h"

static void test_op_00e0(void** state) {
	Chip8 a;
	memset(a.video, 0x01, CHIP8_PIXEL_COUNT);

	op_00e0(&a);

	for (int i = 0; i < CHIP8_PIXEL_COUNT; i++) {
		assert_int_equal(a.video[i], 0);
	}
}

static void test_op_00ee(void** state) {
	Chip8 a;
	a.sp = 0x05;
	int previous_sp_value = a.sp - 1;
	a.stack[previous_sp_value] = 0x10;

	op_00ee(&a);

	assert_int_equal(a.sp, previous_sp_value);
	assert_int_equal(a.pc, a.stack[a.sp]);
}

static void test_op_1nnn(void** state) {
	Chip8 a;
	a.opcode = 0x9006;
	uint16_t address = a.opcode & 0x0fffu;

	op_1nnn(&a);

	assert_int_equal(a.pc, address);
}

static void test_op_2nnn(void** state) {
	Chip8 a;
	uint8_t sp = 0x0au;
	a.sp = sp;
	a.stack[a.sp] = 0;
	a.pc = 0x0020;
	a.opcode = 0x9006;
	uint16_t address = a.opcode & 0x0fffu;

	op_2nnn(&a);

	assert_int_equal(a.sp, sp + 0x01u);
	assert_int_equal(a.stack[sp], 0x0020);
	assert_int_equal(a.pc, address);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_op_00e0),
		cmocka_unit_test(test_op_00ee),
		cmocka_unit_test(test_op_1nnn),
		cmocka_unit_test(test_op_2nnn),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
