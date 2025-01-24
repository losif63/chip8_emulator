#include <stdio.h>
#include <stdlib.h>
#include "register.h"
#include "graphics.h"

int main()
{
    /* Configure chip-8 components */
    char* memory = (char*)malloc(4096);
    register_t* registers = (register_t*)malloc(sizeof(register_t));
    short int* stack = (short int*)malloc(16 * sizeof(short int));

    const int display_width = 64;
    const int display_height = 32;
    int* display = (int*)malloc(display_width * display_height / 64);

    setup_graphics(memory);

    

}