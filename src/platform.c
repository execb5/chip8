#include "../inc/platform.h"

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
						keypad[0x0] = 0xff;
					}
						break;

					case SDLK_1: {
						keypad[0x1] = 0xff;
					}
						break;

					case SDLK_2: {
						keypad[0x2] = 0xff;
					}
						break;

					case SDLK_3: {
						keypad[0x3] = 0xff;
					}
						break;

					case SDLK_q: {
						keypad[0x4] = 0xff;
					}
						break;

					case SDLK_w: {
						keypad[0x5] = 0xff;
					}
						break;

					case SDLK_e: {
						keypad[0x6] = 0xff;
					}
						break;

					case SDLK_a: {
						keypad[0x7] = 0xff;
					}
						break;

					case SDLK_s: {
						keypad[0x8] = 0xff;
					}
						break;

					case SDLK_d: {
						keypad[0x9] = 0xff;
					}
						break;

					case SDLK_z: {
						keypad[0xa] = 0xff;
					}
						break;

					case SDLK_c: {
						keypad[0xb] = 0xff;
					}
						break;

					case SDLK_4: {
						keypad[0xc] = 0xff;
					}
						break;

					case SDLK_r: {
						keypad[0xd] = 0xff;
					}
						break;

					case SDLK_f: {
						keypad[0xe] = 0xff;
					}
						break;

					case SDLK_v: {
						keypad[0xf] = 0xff;
					}
						break;
				}
			}
				break;

			case SDL_KEYUP: {
				switch (event.key.keysym.sym) {
					case SDLK_x: {
						keypad[0x0] = 0x00;
					}
						break;

					case SDLK_1: {
						keypad[0x1] = 0x00;
					}
						break;

					case SDLK_2: {
						keypad[0x2] = 0x00;
					}
						break;

					case SDLK_3: {
						keypad[0x3] = 0x00;
					}
						break;

					case SDLK_q: {
						keypad[0x4] = 0x00;
					}
						break;

					case SDLK_w: {
						keypad[0x5] = 0x00;
					}
						break;

					case SDLK_e: {
						keypad[0x6] = 0x00;
					}
						break;

					case SDLK_a: {
						keypad[0x7] = 0x00;
					}
						break;

					case SDLK_s: {
						keypad[0x8] = 0x00;
					}
						break;

					case SDLK_d: {
						keypad[0x9] = 0x00;
					}
						break;

					case SDLK_z: {
						keypad[0xa] = 0x00;
					}
						break;

					case SDLK_c: {
						keypad[0xb] = 0x00;
					}
						break;

					case SDLK_4: {
						keypad[0xc] = 0x00;
					}
						break;

					case SDLK_r: {
						keypad[0xd] = 0x00;
					}
						break;

					case SDLK_f: {
						keypad[0xe] = 0x00;
					}
						break;

					case SDLK_v: {
						keypad[0xf] = 0x00;
					}
						break;
				}
			}
				break;
		}
	}

	return quit;

}
