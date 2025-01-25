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
void handle_instruction(
    unsigned short instruction,
    char* memory,
    chip8_register* registers,
    unsigned short* stack, 
    unsigned char* display
)
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
                registers->PC = stack[registers->SP--];
            else {
                fprintf(stderr, "Error while decoding: Case 0 / Instruction: 0x%04x", instruction);
                abort();
            }
            break;
        case 0x01:
            registers->PC = instruction & 0x0FFF;
            break;
        case 0x02:
            stack[++registers->SP] = registers->PC;
            registers->PC = instruction & 0x0FFF;
            break;
        case 0x03:
            if(registers->V[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF))
                registers->PC += 2;
            break;
        case 0x04:
            if(registers->V[(instruction & 0x0F00) >> 8] != (instruction & 0x00FF))
                registers->PC += 2;
            break;
        case 0x05:
            if ((instruction & 0x000F) > 0) {
                fprintf(stderr, "Error while decoding: Case 5 / Instruction: 0x%04x", instruction);
                abort();
            }
            if(registers->V[(instruction & 0x0F00) >> 8] != registers->V[(instruction & 0x00F0) >> 4])
                registers->PC += 2;
            break;
        case 0x06:
            registers->V[(instruction & 0x0F00) >> 8] = (char)(instruction & 0x00FF);
            break;
        case 0x07:
            registers->V[(instruction & 0x0F00) >> 8] += (char)(instruction & 0x00FF);
            break;
        case 0x08: {
            uint16_t x = (instruction & 0x0F00) >> 8;
            uint16_t y = (instruction & 0x00F0) >> 4;
            switch(instruction & 0x000F) {
                case 0x00:
                    registers->V[x] = registers->V[y];
                    break;
                case 0x01:
                    registers->V[x] = registers->V[x] | registers->V[y];
                    break;
                case 0x02:
                    registers->V[x] = registers->V[x] & registers->V[y];
                    break;
                case 0x03:
                    registers->V[x] = registers->V[x] ^ registers->V[y];
                    break;
                case 0x04: {
                    unsigned short sum = registers->V[x] + registers->V[y];
                    registers->V[0xF] = (sum > 0xFF) ? 1 : 0;
                    registers->V[x] = (char)sum;
                    break;
                }
                case 0x05:
                    registers->V[0xF] = (registers->V[x] > registers->V[y]) ? 1 : 0;
                    registers->V[x] = registers->V[x] - registers->V[y];
                    break;
                case 0x06:
                    registers->V[0xF] = registers->V[x] & 0x01;
                    registers->V[x] >>= 1;
                    break;
                case 0x07:
                    registers->V[0xF] = (registers->V[y] > registers->V[x]) ? 1 : 0;
                    registers->V[x] = registers->V[y] - registers->V[x];
                    break;
                case 0x0E:
                    registers->V[0xF] = (registers->V[x] & 0x80) >> 7;
                    registers->V[x] <<= 1;
                    break;
            }
            break;
        }
        case 0x09: {
            if((instruction & 0x000F) > 0) {
                fprintf(stderr, "Error while decoding: Case 9 / Instruction: 0x%04x", instruction);
                abort();
            }
            unsigned short x = (instruction & 0x0F00) >> 8;
            unsigned short y = (instruction & 0x00F0) >> 4;
            if(registers->V[x] != registers->V[y])
                registers->PC += 2;
            break;
        }
        case 0x0A:
            registers->I = instruction & 0x0FFF;
            break;
        case 0x0B:
            registers->PC = (instruction & 0x0FFF) + registers->V[0];
            break;
        case 0x0C: {
            unsigned short x = (instruction & 0x0F00) >> 8;
            registers->V[x] = (char)(rand() % 256) & (instruction & 0x00FF);
            break;
        }
        case 0x0D: {
            unsigned short x = (instruction & 0x0F00) >> 8;
            unsigned short y = (instruction & 0x00F0) >> 4;
            unsigned short n = (instruction & 0x000F);
            registers->V[0xF] = 0;      // Reset collision flag
            for (int i = 0; i < n; i++) {
                unsigned char cursor = memory[registers->I + i];
                for (int j = 0; j < 8; j++) {
                    int pixel_x = (registers->V[x] + j) % CHIP8_WIDTH;
                    int pixel_y = (registers->V[y] + i) % CHIP8_HEIGHT;
                    int sprite_pixel = (cursor >> (7 - j)) & 1;
                    int display_pixel = get_pixel(pixel_x, pixel_y, display);
                    if (sprite_pixel && display_pixel)
                        registers->V[0xF] = 1;
                    set_pixel(pixel_x, pixel_y, display, display_pixel ^ sprite_pixel);
                }
            }
            break;
        }
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
    unsigned short* stack = (unsigned short*)malloc(16 * sizeof(unsigned short));

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