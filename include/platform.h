#include </opt/homebrew/include/SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

void platform_create(char* title, int window_width, int window_height, int texture_width, int texture_height);
void platform_destroy(void);
void platform_update(void* buffer, int pitch);
int platform_process_input(uint8_t* keypad);
