#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "register.h"
#include "graphics.h"

/* TODO: Need to use memory and stack in this function */
void handle_instruction(unsigned short instruction, chip8_register* reg, unsigned char* display)
{
    unsigned short digit1 = (instruction & 0xF000) >> 12;
    unsigned short digit2 = (instruction & 0x0F00) >> 8;
    unsigned short digit3 = (instruction & 0x00F0) >> 4;
    unsigned short digit4 = (instruction & 0x000F);
    switch(digit1) {
        case 0x00:
            if(instruction == 0x00E0) 
                memset(display, 0, CHIP8_WIDTH * CHIP8_HEIGHT / 8);
            /* TODO: This implementation is wrong */
            else if(instruction == 0x00EE)
                reg->PC = (unsigned short)reg->SP--;
            else {
                fprintf(stderr, "Error while decoding: Case 0 / Instruction: 0x%04x", instruction);
            }
            break;
        case 0x01:
            reg->PC = instruction & 0x0FFF;
            break;
        case 0x02:

            break;
        case 0x03:
            break;
        case 0x04:
            break;
        case 0x05:
            break;
        case 0x06:
            break;
        case 0x07:
            break;
        case 0x08:
            break;
        case 0x09:
            break;
        case 0x0A:
            break;
        case 0x0B:
            break;
        case 0x0C:
            break;
        case 0x0D:
            break;
        case 0x0E:
            break;
        case 0x0F:
            break;
    }
}

int main(int argc, char** argv)
{
    SDL_Window* window;

    /* Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    /* Configure chip-8 components */
    char* memory = (char*)malloc(4096);
    chip8_register* registers = (chip8_register*)malloc(sizeof(chip8_register));
    short int* stack = (short int*)malloc(16 * sizeof(short int));

    /* Display */
    unsigned char* display = (unsigned char*)malloc(CHIP8_WIDTH * CHIP8_HEIGHT / 8);
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

    /* Read in chip8 ROM file */
    char* filename = "roms/games/Cave.ch8";
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        printf("Read error\n");
        return 1;
    }

    /* IMPORTANT: BUFFER SHOULD BE UNSIGNED CHAR */
    /* OTHERWISE, FACE SERIOUS OVERFLOW ISSUES */
    unsigned short read_buf;
    while(read(fd, &read_buf, 2) == 2) {
        unsigned short instruction = (read_buf << 8) | (read_buf >> 8);
        printf("0x%04x\n", instruction);
    }

    // /* Main loop */
    // SDL_Event event;
    // int key = 0;
    // while(1) {
    //     if(SDL_PollEvent(&event)) {
    //         if(event.type == SDL_EVENT_QUIT) {
    //             break;
    //         }
    //     }

    //     SDL_RenderClear(renderer);
    //     for(int y = 0; y < CHIP8_HEIGHT; y++) {
    //         for(int x = 0; x < CHIP8_WIDTH; x++) {
    //             SDL_Texture* texture = get_pixel(x, y, display) ? texture_on : texture_off;
    //             SDL_FRect frect = {x * 10, y * 10, 10, 10};
    //             SDL_RenderTexture(renderer, texture, NULL, &frect);
    //         }
    //     }
    //     set_pixel(10, 10, display, key % 2);
    //     key += 1;
    //     SDL_RenderPresent(renderer);
    //     SDL_Delay(50);
    // }

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