#include "graphics.h"
#include "SDL3/SDL_video.h"

void setup_graphics(char* memory)
{
    /* Initialize sprites in interpreter memory */
    *(memory + SPRITE_ADDR(0) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(0) + 1) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(0) + 2) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(0) + 3) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(0) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(1) + 0) = 0x20;          /* 00100000 */
    *(memory + SPRITE_ADDR(1) + 1) = 0x60;          /* 01100000 */
    *(memory + SPRITE_ADDR(1) + 2) = 0x20;          /* 00100000 */
    *(memory + SPRITE_ADDR(1) + 3) = 0x20;          /* 00100000 */
    *(memory + SPRITE_ADDR(1) + 4) = 0x70;          /* 01110000 */

    *(memory + SPRITE_ADDR(2) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(2) + 1) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(2) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(2) + 3) = 0x80;          /* 10000000 */
    *(memory + SPRITE_ADDR(2) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(3) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(3) + 1) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(3) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(3) + 3) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(3) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(4) + 0) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(4) + 1) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(4) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(4) + 3) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(4) + 4) = 0x10;          /* 00010000 */

    *(memory + SPRITE_ADDR(5) + 0) = 0xF0;          /* 11110000 */    
    *(memory + SPRITE_ADDR(5) + 1) = 0x80;          /* 10000000 */
    *(memory + SPRITE_ADDR(5) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(5) + 3) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(5) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(6) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(6) + 1) = 0x80;          /* 10000000 */
    *(memory + SPRITE_ADDR(6) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(6) + 3) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(6) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(7) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(7) + 1) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(7) + 2) = 0x20;          /* 00100000 */
    *(memory + SPRITE_ADDR(7) + 3) = 0x40;          /* 01000000 */
    *(memory + SPRITE_ADDR(7) + 4) = 0x40;          /* 01000000 */

    *(memory + SPRITE_ADDR(8) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(8) + 1) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(8) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(8) + 3) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(8) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(9) + 0) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(9) + 1) = 0x90;          /* 10010000 */
    *(memory + SPRITE_ADDR(9) + 2) = 0xF0;          /* 11110000 */
    *(memory + SPRITE_ADDR(9) + 3) = 0x10;          /* 00010000 */
    *(memory + SPRITE_ADDR(9) + 4) = 0xF0;          /* 11110000 */

    *(memory + SPRITE_ADDR(10) + 0) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(10) + 1) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(10) + 2) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(10) + 3) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(10) + 4) = 0x90;         /* 10010000 */

    *(memory + SPRITE_ADDR(11) + 0) = 0xE0;         /* 11100000 */
    *(memory + SPRITE_ADDR(11) + 1) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(11) + 2) = 0xE0;         /* 11100000 */
    *(memory + SPRITE_ADDR(11) + 3) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(11) + 4) = 0xE0;         /* 11100000 */

    *(memory + SPRITE_ADDR(12) + 0) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(12) + 1) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(12) + 2) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(12) + 3) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(12) + 4) = 0xF0;         /* 11110000 */

    *(memory + SPRITE_ADDR(13) + 0) = 0xE0;         /* 11100000 */
    *(memory + SPRITE_ADDR(13) + 1) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(13) + 2) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(13) + 3) = 0x90;         /* 10010000 */
    *(memory + SPRITE_ADDR(13) + 4) = 0xE0;         /* 11100000 */

    *(memory + SPRITE_ADDR(14) + 0) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(14) + 1) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(14) + 2) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(14) + 3) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(14) + 4) = 0xF0;         /* 11110000 */

    *(memory + SPRITE_ADDR(15) + 0) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(15) + 1) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(15) + 2) = 0xF0;         /* 11110000 */
    *(memory + SPRITE_ADDR(15) + 3) = 0x80;         /* 10000000 */
    *(memory + SPRITE_ADDR(15) + 4) = 0x80;         /* 10000000 */
}

int get_pixel(int x, int y, unsigned char* display)
{
    int index = y * CHIP8_WIDTH + x;
    int shift = 7 - index % 8;
    return display[index / 8] & (1 << shift);
}

void set_pixel(int x, int y, unsigned char* display, int val)
{
    int index = y * CHIP8_WIDTH + x;
    int shift = 7 - index % 8;
    if (val) display[index / 8] |= (1 << shift);
    else display[index / 8] &= ~(1 << shift);
    return;
}

void draw_display(int* display, const int display_width, const int display_height)
{
    return;
}

void debug_display(unsigned char* display)
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