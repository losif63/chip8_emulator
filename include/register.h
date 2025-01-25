#ifndef REGISTER_H
#define REGISTER_H

typedef struct {
    /* General Purpose Registers */
    char V[16];
    short int I;

    /* Special Purpose Registers */
    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short int PC;   /* program counter */
    unsigned char SP;        /* stack pointer */
} chip8_register;

chip8_register* init_chip8_register();

#endif