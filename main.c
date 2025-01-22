#include <stdio.h>
#include <stdlib.h>

typedef struct {
    /* General Purpose Registers */
    char V0;
    char V1;
    char V2;
    char V3;
    char V4;
    char V5;
    char V6;
    char V7;
    char V8;
    char V9;
    char VA;
    char VB;
    char VC;
    char VD;
    char VE;
    char VF;
    
    short int I;

    /* Special Purpose Registers */
    char delay_timer;
    char sound_timer;

    short int PC;   /* program counter */
    char SP;        /* stack pointer */
} register_t;

int get_sprite_addr(int sprite)
{
    return 5 * sprite;
}

void setup_graphics(char* memory)
{
    /* Initialize sprites in interpreter memory */
    *(memory + get_sprite_addr(0) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(0) + 1) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(0) + 2) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(0) + 3) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(0) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(1) + 0) = 0x20;      /* 00100000 */
    *(memory + get_sprite_addr(1) + 1) = 0x60;      /* 01100000 */
    *(memory + get_sprite_addr(1) + 2) = 0x20;      /* 00100000 */
    *(memory + get_sprite_addr(1) + 3) = 0x20;      /* 00100000 */
    *(memory + get_sprite_addr(1) + 4) = 0x70;      /* 01110000 */

    *(memory + get_sprite_addr(2) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(2) + 1) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(2) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(2) + 3) = 0x80;      /* 10000000 */
    *(memory + get_sprite_addr(2) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(3) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(3) + 1) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(3) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(3) + 3) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(3) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(4) + 0) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(4) + 1) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(4) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(4) + 3) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(4) + 4) = 0x10;      /* 00010000 */

    *(memory + get_sprite_addr(5) + 0) = 0xF0;      /* 11110000 */    
    *(memory + get_sprite_addr(5) + 1) = 0x80;      /* 10000000 */
    *(memory + get_sprite_addr(5) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(5) + 3) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(5) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(6) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(6) + 1) = 0x80;      /* 10000000 */
    *(memory + get_sprite_addr(6) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(6) + 3) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(6) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(7) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(7) + 1) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(7) + 2) = 0x20;      /* 00100000 */
    *(memory + get_sprite_addr(7) + 3) = 0x40;      /* 01000000 */
    *(memory + get_sprite_addr(7) + 4) = 0x40;      /* 01000000 */

    *(memory + get_sprite_addr(8) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(8) + 1) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(8) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(8) + 3) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(8) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(9) + 0) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(9) + 1) = 0x90;      /* 10010000 */
    *(memory + get_sprite_addr(9) + 2) = 0xF0;      /* 11110000 */
    *(memory + get_sprite_addr(9) + 3) = 0x10;      /* 00010000 */
    *(memory + get_sprite_addr(9) + 4) = 0xF0;      /* 11110000 */

    *(memory + get_sprite_addr(10) + 0) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(10) + 1) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(10) + 2) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(10) + 3) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(10) + 4) = 0x90;     /* 10010000 */

    *(memory + get_sprite_addr(11) + 0) = 0xE0;     /* 11100000 */
    *(memory + get_sprite_addr(11) + 1) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(11) + 2) = 0xE0;     /* 11100000 */
    *(memory + get_sprite_addr(11) + 3) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(11) + 4) = 0xE0;     /* 11100000 */

    *(memory + get_sprite_addr(12) + 0) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(12) + 1) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(12) + 2) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(12) + 3) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(12) + 4) = 0xF0;     /* 11110000 */

    *(memory + get_sprite_addr(13) + 0) = 0xE0;     /* 11100000 */
    *(memory + get_sprite_addr(13) + 1) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(13) + 2) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(13) + 3) = 0x90;     /* 10010000 */
    *(memory + get_sprite_addr(13) + 4) = 0xE0;     /* 11100000 */

    *(memory + get_sprite_addr(14) + 0) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(14) + 1) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(14) + 2) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(14) + 3) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(14) + 4) = 0xF0;     /* 11110000 */

    *(memory + get_sprite_addr(15) + 0) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(15) + 1) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(15) + 2) = 0xF0;     /* 11110000 */
    *(memory + get_sprite_addr(15) + 3) = 0x80;     /* 10000000 */
    *(memory + get_sprite_addr(15) + 4) = 0x80;     /* 10000000 */
}

void draw_display(int* display, const int display_width, const int display_height)
{
    
}

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