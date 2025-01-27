#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SPRITE_ADDR(sprite) (5 * (sprite))

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

void setup_graphics(unsigned char* memory);

int get_pixel(int x, int y, unsigned char* display);

void set_pixel(int x, int y, unsigned char* display, int val);

void draw_display(int* display, int display_width, int display_height);

void debug_display(unsigned char* display);

#endif