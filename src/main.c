#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "register.h"
#include "graphics.h"

int main(int argc, char** argv)
{
    SDL_Window* window;

    /* Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    /* Configure chip-8 components */
    char* memory = (char*)malloc(4096);
    register_t* registers = (register_t*)malloc(sizeof(register_t));
    short int* stack = (short int*)malloc(16 * sizeof(short int));

    const int display_width = 64;
    const int display_height = 32;
    int* display = (int*)malloc(display_width * display_height / 64);

    setup_graphics(memory);

    /* Create window */
    window = SDL_CreateWindow(
        "Chip-8 Emulator",
        640,
        320,
        SDL_WINDOW_OPENGL
    );
    if(window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    /* Main loop */
    while(1) {
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                break;
            }
        }
    }


    /* Destroy window */
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