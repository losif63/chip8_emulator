#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stdlib.h>
#include <SDL3/SDL.h>
#include "register.h"

#define CHIP8_MEMORY 4096
#define CHIP8_STACK 16


char code_to_num(char code);

char num_to_code(char Vx);

void dump_state(unsigned char* memory, chip8_register* registers, unsigned short* stack,  unsigned char* display);

#endif