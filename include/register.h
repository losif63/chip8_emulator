#ifndef REGISTER_H
#define REGISTER_H

typedef struct {
    /* General Purpose Registers */
    char V[16];
    short int I;

    /* Special Purpose Registers */
    char delay_timer;
    char sound_timer;

    short int PC;   /* program counter */
    char SP;        /* stack pointer */
} chip8_register;


#endif