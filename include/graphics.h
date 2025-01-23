#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SPRITE_ADDR(sprite) (5 * (sprite))

void setup_graphics(char* memory);

void draw_display(int* display, int display_width, int display_height);

#endif