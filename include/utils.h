#ifndef UTILS_H
#define UTILS_H

#define SPRITE_ADDR(sprite) (5 * (sprite))

void setup_graphics(char* memory);

void draw_display(int* display, int display_width, int display_height);

#endif