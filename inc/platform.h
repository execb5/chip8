#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <SDL.h>

extern SDL_Window* platform_window;
extern SDL_Renderer* platform_renderer;
extern SDL_Texture* platform_texture;

void platform_create(char* title, int window_width, int window_height, int texture_width, int texture_height);
void platform_destroy(void);
void platform_update(uint32_t* buffer, int pitch);
int platform_process_input(uint8_t* keypad);

#endif /* PLATFORM_H */
