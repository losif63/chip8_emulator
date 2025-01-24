#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "register.h"
#include "graphics.h"

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

int get_pixel(int x, int y, char* display)
{
    int index = y * CHIP8_WIDTH + x;
    int shift = 7 - index % 8;
    return display[index / 8] & (1 << shift);
}

void set_pixel(int x, int y, char* display, int val)
{
    int index = y * CHIP8_WIDTH + x;
    int shift = 7 - index % 8;
    if (val) display[index / 8] |= (1 << shift);
    else display[index / 8] &= ~(1 << shift);
    return;
}

void debug_display(char* display)
{
    for (int y = 0; y < CHIP8_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_WIDTH; x++) {
            int index = y * CHIP8_WIDTH + x;
            printf("%d ", display[index / 8] & (1 << 7));
            printf("%d ", display[index / 8] & (1 << 6));
            printf("%d ", display[index / 8] & (1 << 5));
            printf("%d ", display[index / 8] & (1 << 4));
            printf("%d ", display[index / 8] & (1 << 3));
            printf("%d ", display[index / 8] & (1 << 2));
            printf("%d ", display[index / 8] & (1 << 1));
            printf("%d ", display[index / 8] & (1 << 0));
        }
        printf("\n");
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
    char* display = (char*)malloc(CHIP8_WIDTH * CHIP8_HEIGHT / 8);
    memset(display, 0, CHIP8_WIDTH * CHIP8_HEIGHT / 8);

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
    SDL_Texture* texture_on = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA32, 
        SDL_TEXTUREACCESS_STATIC, 
        10, 
        10
    );

    SDL_Texture* texture_off = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STATIC,
        10,
        10
    );

    void* on_buffer = malloc(100 * sizeof(uint32_t));
    void* off_buffer = malloc(100 * sizeof(uint32_t));
    memset(on_buffer, 0xFF, 100 * sizeof(uint32_t));
    memset(off_buffer, 0x00, 100 * sizeof(uint32_t));

    SDL_UpdateTexture(texture_on, NULL, on_buffer, 10 * sizeof(uint32_t));
    SDL_UpdateTexture(texture_off, NULL, off_buffer, 10 * sizeof(uint32_t));

    /* Main loop */
    SDL_Event event;
    int key = 0;
    while(1) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                break;
            }
        }

        SDL_RenderClear(renderer);
        for(int y = 0; y < CHIP8_HEIGHT; y++) {
            for(int x = 0; x < CHIP8_WIDTH; x++) {
                SDL_Texture* texture = get_pixel(x, y, display) ? texture_on : texture_off;
                SDL_FRect frect = {x * 10, y * 10, 10, 10};
                SDL_RenderTexture(renderer, texture, NULL, &frect);
            }
        }
        set_pixel(10, 10, display, key % 2);
        key += 1;
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    /* Destroy window */
    SDL_DestroyTexture(texture_on);
    SDL_DestroyTexture(texture_off);
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