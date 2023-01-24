#include "../include/platform.h"

SDL_Window* platform_window = NULL;
SDL_Renderer* platform_renderer = NULL;
SDL_Texture* platform_texture = NULL;

void platform_create(char* title, int window_width, int window_height, int texture_width, int texture_height) {
	SDL_Init(SDL_INIT_VIDEO);
	platform_window = SDL_CreateWindow(title, 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);
	platform_renderer = SDL_CreateRenderer(platform_window, -1, SDL_RENDERER_ACCELERATED);
	platform_texture = SDL_CreateTexture(platform_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_width, texture_height);
}

void platform_destroy(void) {
	SDL_DestroyTexture(platform_texture);
	SDL_DestroyRenderer(platform_renderer);
	SDL_DestroyWindow(platform_window);
	SDL_Quit();
}

void platform_update(uint32_t* buffer, int pitch) {
	SDL_UpdateTexture(platform_texture, NULL, buffer, pitch);
	SDL_RenderClear(platform_renderer);
	SDL_RenderCopy(platform_renderer, platform_texture, NULL, NULL);
	SDL_RenderPresent(platform_renderer);
}

int platform_process_input(uint8_t* keypad) {
	int quit = 0;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				quit = 1;
			}
				break;

			case SDL_KEYDOWN: {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: {
						quit = 1;
					}
						break;

					case SDLK_x: {
						keypad[0] = 1;
					}
						break;

					case SDLK_1: {
						keypad[1] = 1;
					}
						break;

					case SDLK_2: {
						keypad[2] = 1;
					}
						break;

					case SDLK_3: {
						keypad[3] = 1;
					}
						break;

					case SDLK_q: {
						keypad[4] = 1;
					}
						break;

					case SDLK_w: {
						keypad[5] = 1;
					}
						break;

					case SDLK_e: {
						keypad[6] = 1;
					}
						break;

					case SDLK_a: {
						keypad[7] = 1;
					}
						break;

					case SDLK_s: {
						keypad[8] = 1;
					}
						break;

					case SDLK_d: {
						keypad[9] = 1;
					}
						break;

					case SDLK_z: {
						keypad[0xA] = 1;
					}
						break;

					case SDLK_c: {
						keypad[0xB] = 1;
					}
						break;

					case SDLK_4: {
						keypad[0xC] = 1;
					}
						break;

					case SDLK_r: {
						keypad[0xD] = 1;
					}
						break;

					case SDLK_f: {
						keypad[0xE] = 1;
					}
						break;

					case SDLK_v: {
						keypad[0xF] = 1;
					}
						break;
				}
			}
				break;

			case SDL_KEYUP: {
				switch (event.key.keysym.sym) {
					case SDLK_x: {
						keypad[0] = 0;
					}
						break;

					case SDLK_1: {
						keypad[1] = 0;
					}
						break;

					case SDLK_2: {
						keypad[2] = 0;
					}
						break;

					case SDLK_3: {
						keypad[3] = 0;
					}
						break;

					case SDLK_q: {
						keypad[4] = 0;
					}
						break;

					case SDLK_w: {
						keypad[5] = 0;
					}
						break;

					case SDLK_e: {
						keypad[6] = 0;
					}
						break;

					case SDLK_a: {
						keypad[7] = 0;
					}
						break;

					case SDLK_s: {
						keypad[8] = 0;
					}
						break;

					case SDLK_d: {
						keypad[9] = 0;
					}
						break;

					case SDLK_z: {
						keypad[0xA] = 0;
					}
						break;

					case SDLK_c: {
						keypad[0xB] = 0;
					}
						break;

					case SDLK_4: {
						keypad[0xC] = 0;
					}
						break;

					case SDLK_r: {
						keypad[0xD] = 0;
					}
						break;

					case SDLK_f: {
						keypad[0xE] = 0;
					}
						break;

					case SDLK_v: {
						keypad[0xF] = 0;
					}
						break;
				}
			}
				break;
		}
	}

	return quit;

}
