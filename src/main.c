#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "register.h"
#include "graphics.h"

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

/* Function for converting display bitmap into greyscale buffer */
void display_to_buffer(char* display, uint32_t* display_buffer) {
    memset(display_buffer, 0, CHIP8_WIDTH * CHIP8_HEIGHT * sizeof(uint32_t));
    for (int i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; i++) {
        display_buffer[i] = display[i / 8] & (1 << (i % 8)) ? 0xFFFFFFFF : 0x00000000;
    }
}

int main(int argc, char** argv)
{
    SDL_Window* window;

    /* Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    /* Configure chip-8 components */
    char* memory = (char*)malloc(4096);
    register_t* registers = (register_t*)malloc(sizeof(register_t));
    short int* stack = (short int*)malloc(16 * sizeof(short int));

    /* Display */
    char* display = (char*)malloc(CHIP8_WIDTH * CHIP8_HEIGHT / 64);
    uint32_t* display_buffer = (uint32_t*)malloc(CHIP8_WIDTH * CHIP8_HEIGHT * sizeof(uint32_t));
    memset(display, 0, CHIP8_WIDTH * CHIP8_HEIGHT / 64);
    memset(display_buffer, 0, CHIP8_WIDTH * CHIP8_HEIGHT * sizeof(uint32_t));

    setup_graphics(memory);

    /* Create window */
    window = SDL_CreateWindow(
        "Chip-8 Emulator",
        CHIP8_WIDTH * 10,
        CHIP8_HEIGHT * 10,
        SDL_WINDOW_OPENGL
    );
    if(window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    /* Create renderer */
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "opengl");

    /* Create bitmap texture for chip-8 */
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA32, 
        SDL_TEXTUREACCESS_STREAMING, 
        CHIP8_WIDTH, 
        CHIP8_HEIGHT
    );

    /* Main loop */
    while(1) {
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                break;
            }
        }
        display_to_buffer(display, display_buffer);
        SDL_UpdateTexture(texture, NULL, display_buffer, CHIP8_WIDTH * CHIP8_HEIGHT * sizeof(uint32_t));
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        display[0] = ~display[0];
        SDL_Delay(200);
    }

    /* Destroy window */
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /* Free memory */
    free(memory);
    free(registers);
    free(stack);
    free(display);

    /* Quit SDL */
    SDL_Quit();
    return 0;
}