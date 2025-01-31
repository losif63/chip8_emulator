#include "utils.h"

char code_to_num(char code) {
    switch (code) {
        case SDL_SCANCODE_1:
            return 1;
        case SDL_SCANCODE_2:
            return 2;
        case SDL_SCANCODE_3:
            return 3;
        case SDL_SCANCODE_4:
            return 0xC;
        case SDL_SCANCODE_Q:
            return 4;
        case SDL_SCANCODE_W:
            return 5;
        case SDL_SCANCODE_E:
            return 6;
        case SDL_SCANCODE_R:
            return 0xD;
        case SDL_SCANCODE_A:
            return 7;
        case SDL_SCANCODE_S:
            return 8;
        case SDL_SCANCODE_D:
            return 9;
        case SDL_SCANCODE_F:
            return 0xE;
        case SDL_SCANCODE_Z:
            return 0xA;
        case SDL_SCANCODE_X:
            return 0;
        case SDL_SCANCODE_C:
            return 0xB;
        case SDL_SCANCODE_V:
            return 0xF;
        default:
            fprintf(stderr, "Error while getting key num: 0x%02x", code);
            abort();
    }
}

char num_to_code(char Vx) {
    switch (Vx) {
        case 0x0:
            return SDL_SCANCODE_X;
        case 0x1:
            return SDL_SCANCODE_1;
        case 0x2:
            return SDL_SCANCODE_2;
        case 0x3:
            return SDL_SCANCODE_3;
        case 0x4:
            return SDL_SCANCODE_Q;
        case 0x5:
            return SDL_SCANCODE_W;
        case 0x6:
            return SDL_SCANCODE_E;
        case 0x7:
            return SDL_SCANCODE_A;
        case 0x8:
            return SDL_SCANCODE_S;
        case 0x9:
            return SDL_SCANCODE_D;
        case 0xA:
            return SDL_SCANCODE_Z;
        case 0xB:
            return SDL_SCANCODE_C;
        case 0xC:
            return SDL_SCANCODE_4;
        case 0xD:
            return SDL_SCANCODE_R;
        case 0xE:
            return SDL_SCANCODE_F;
        case 0xF:
            return SDL_SCANCODE_V;
        default:
            fprintf(stderr, "Error while getting sdl code: 0x%02x", Vx);
            abort();
    }
}

void dump_state(unsigned char* memory, chip8_register* registers, unsigned short* stack, unsigned char* display)
{
    /* Print register state */
    printf("REGISTER\n");
    for(int i=0; i<16; i++) printf("   V%X  ", i); printf("    VI   "); printf("    PC   "); printf("   SP  ");
    printf("\n+");
    for(int i=0; i<16; i++) printf("------+"); printf("--------+"); printf("--------+"); printf("------+");
    printf("\n|");
    for(int i=0; i<16; i++) printf(" 0x%02X |", registers->V[i]); printf(" 0x%04X |", registers->I); printf(" 0x%04X |", registers->PC); printf(" 0x%02X |", registers->SP);
    printf("\n+");
    for(int i=0; i<16; i++) printf("------+"); printf("--------+"); printf("--------+"); printf("------+");
    printf("\n");

    /* Print stack state */
    printf("STACK\n");
    for(int i=0; i<16; i++) printf("  0x%04X ", i);
    printf("\n+");
    for(int i=0; i<16; i++) printf("--------+");
    printf("\n|");
    for(int i=0; i<16; i++) printf(" 0x%04X |", stack[i]);
    printf("\n+");
    for(int i=0; i<16; i++) printf("--------+");
    printf("\n");
    
    /* Print memory state */
    printf("MEMORY\n");
    // printf("+");
    // for(int i=0; i<CHIP8_MEMORY; i++) printf("------+");
    // printf("\n|");
    // for(int i=0; i<CHIP8_MEMORY; i++) printf(" 0x%02X |", memory[i]);
    // printf("\n+");
    // for(int i=0; i<CHIP8_MEMORY; i++) printf("------+");
    // printf("\n ");
    // for(int i=0; i<CHIP8_MEMORY; i++) printf("0x%04X ", i);
    // printf("\n\n");
    
    /* Print display state */
    

    printf("\n\n");
    return;
}