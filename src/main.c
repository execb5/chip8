#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/instructions.h"
#include "../inc/platform.h"

#define NUMBER_OF_ARGUMENTS 4
#define TITLE "My Cute Chip8 Emulator"

int main(int argc, char** argv) {
	if (argc != NUMBER_OF_ARGUMENTS) {
		printf("Wrong number of arguments.\n");
		printf("Expected %d, but got %d\n", NUMBER_OF_ARGUMENTS, argc - 1);
		printf("Usage: %s <scale> <delay> <rom>\n", argv[0]);
		return 1;
	}


	int video_scale = atoi(argv[1]);
	int cycle_delay = atoi(argv[2]);
	char* rom_file = argv[3];

	platform_create(TITLE, CHIP8_SCREEN_WIDTH * video_scale, CHIP8_SCREEN_HEIGHT * video_scale, CHIP8_SCREEN_WIDTH, CHIP8_SCREEN_HEIGHT);

	Chip8* chip = create();
	load_rom(chip, rom_file);

	int video_pitch = sizeof(chip->video[0]) * CHIP8_SCREEN_WIDTH;

	clock_t last_cycle_time = clock();
	int quit = 0;

	while(!quit) {
		quit = platform_process_input(chip->keypad);

		clock_t current_time = clock();
		int dt = (current_time - last_cycle_time) * 1000 / CLOCKS_PER_SEC;

		if (dt > cycle_delay) {
			last_cycle_time = current_time;
			cycle(chip);
			platform_update(chip->video, video_pitch);
		}
	}

	destroy(chip);
	platform_destroy();

	return 0;
}
