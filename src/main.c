#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_error.h"
#include "graphics.h"
#include "register.h"
#include "tinyfiledialogs.h"
#include "utils.h"

#define SAMPLING_FREQUENCY 44100
#define BEEP_FREQUENCY 440
#define LOUDNESS 4000

void handle_instruction(unsigned short instruction, unsigned char *memory,
                        chip8_register *registers, unsigned short *stack,
                        unsigned char *display, const bool *keys,
                        SDL_AudioStream *stream, SDL_Event *event) {
  uint16_t digit1 = (instruction & 0xF000) >> 12;
  uint16_t x = (instruction & 0x0F00) >> 8;
  uint16_t y = (instruction & 0x00F0) >> 4;
  uint16_t n = instruction & 0x000F;
  uint16_t nn = instruction & 0x00FF;
  uint16_t nnn = instruction & 0x0FFF;
  switch (digit1) {
  case 0x00:
    if (nnn == 0x00E0)
      memset(display, 0, CHIP8_WIDTH * CHIP8_HEIGHT / 8);
    else if (nnn == 0x00EE)
      registers->PC = stack[registers->SP--];
    else {
      fprintf(stderr, "Error while decoding: Case 0 / Instruction: 0x%04x",
              instruction);
      abort();
    }
    break;
  case 0x01:
    registers->PC = nnn;
    break;
  case 0x02:
    stack[++registers->SP] = registers->PC;
    registers->PC = nnn;
    break;
  case 0x03:
    if (registers->V[x] == nn)
      registers->PC += 2;
    break;
  case 0x04:
    if (registers->V[x] != nn)
      registers->PC += 2;
    break;
  case 0x05:
    if (n > 0) {
      fprintf(stderr, "Error while decoding: Case 5 / Instruction: 0x%04x",
              instruction);
      abort();
    }
    if (registers->V[x] != registers->V[y])
      registers->PC += 2;
    break;
  case 0x06:
    registers->V[x] = (char)nn;
    break;
  case 0x07:
    registers->V[x] += (char)nn;
    break;
  case 0x08:
    switch (instruction & 0x000F) {
    case 0x00:
      registers->V[x] = registers->V[y];
      break;
    case 0x01:
      registers->V[x] = registers->V[x] | registers->V[y];
      break;
    case 0x02:
      registers->V[x] = registers->V[x] & registers->V[y];
      break;
    case 0x03:
      registers->V[x] = registers->V[x] ^ registers->V[y];
      break;
    case 0x04: {
      unsigned short sum = registers->V[x] + registers->V[y];
      registers->V[0xF] = (sum > 0xFF) ? 1 : 0;
      registers->V[x] = (char)sum;
      break;
    }
    case 0x05:
      registers->V[0xF] = (registers->V[x] > registers->V[y]) ? 1 : 0;
      registers->V[x] = registers->V[x] - registers->V[y];
      break;
    case 0x06:
      registers->V[0xF] = registers->V[x] & 0x01;
      registers->V[x] >>= 1;
      break;
    case 0x07:
      registers->V[0xF] = (registers->V[y] > registers->V[x]) ? 1 : 0;
      registers->V[x] = registers->V[y] - registers->V[x];
      break;
    case 0x0E:
      registers->V[0xF] = (registers->V[x] & 0x80) >> 7;
      registers->V[x] <<= 1;
      break;
    }
    break;
  case 0x09:
    if ((instruction & 0x000F) > 0) {
      fprintf(stderr, "Error while decoding: Case 9 / Instruction: 0x%04x",
              instruction);
      abort();
    }
    if (registers->V[x] != registers->V[y])
      registers->PC += 2;
    break;
  case 0x0A:
    registers->I = instruction & 0x0FFF;
    break;
  case 0x0B:
    registers->PC = (instruction & 0x0FFF) + registers->V[0];
    break;
  case 0x0C:
    registers->V[x] = (char)(rand() % 256) & (instruction & 0x00FF);
    break;
  case 0x0D:
    registers->V[0xF] = 0; // Reset collision flag
    for (int i = 0; i < n; i++) {
      unsigned char cursor = memory[registers->I + i];
      for (int j = 0; j < 8; j++) {
        int pixel_x = (registers->V[x] + j) % CHIP8_WIDTH;
        int pixel_y = (registers->V[y] + i) % CHIP8_HEIGHT;
        int sprite_pixel = (cursor >> (7 - j)) & 1;
        int display_pixel = get_pixel(pixel_x, pixel_y, display);
        if (sprite_pixel && display_pixel)
          registers->V[0xF] = 1;
        /* BUG ORIGIN */ /* INT BOOL CONVERSION IMPORTANT */
        set_pixel(pixel_x, pixel_y, display,
                  ((bool)display_pixel) ^ sprite_pixel);
      }
    }
    break;
  case 0x0E:
    if ((instruction & 0x00FF) == 0x9E) {
      if (keys[num_to_code(registers->V[x])])
        registers->PC += 2;
    } else if ((instruction & 0xFF) == 0xA1) {
      if (!keys[num_to_code(registers->V[x])])
        registers->PC += 2;

    } else {
      fprintf(stderr, "Error while decoding: Case E / Instruction: 0x%04x",
              instruction);
      abort();
    }
    break;
  case 0x0F:
    switch (nn) {
    case 0x07:
      registers->V[x] = registers->delay_timer;
      break;
    case 0x0A:
      if ((*event).type == SDL_EVENT_KEY_DOWN)
        registers->V[x] = code_to_num(event->key.scancode);
      else
        registers->PC -= 2;
      break;
    case 0x15:
      registers->delay_timer = registers->V[x];
      break;
    case 0x18:
      registers->sound_timer = registers->V[x];
      break;
    case 0x1E:
      registers->I += registers->V[x];
      break;
    case 0x29:
      registers->I = SPRITE_ADDR(registers->V[x]);
      break;
    case 0x33:
      memory[registers->I] = registers->V[x] / 100;
      memory[registers->I + 1] = (registers->V[x] / 10) % 10;
      memory[registers->I + 2] = registers->V[x] % 10;
      break;
    case 0x55: {
      for (int i = 0; i <= x; i++)
        memory[registers->I + i] = registers->V[i];
      break;
    }
    case 0x65: {
      for (int i = 0; i <= x; i++)
        registers->V[i] = memory[registers->I + i];
      break;
    }
    default:
      fprintf(stderr, "Error while decoding: Case F / Instruction: 0x%04x",
              instruction);
      abort();
    }
    break;
  }
}

void add_sinusoidal_audio(SDL_AudioStream *stream) {
  /* Create 1000ms of 440Hz beep sound buffer */
  short *sound_buffer = (short *)malloc(SAMPLING_FREQUENCY * sizeof(short));
  for (int i = 0; i < SAMPLING_FREQUENCY; i++) {
    float audio_value =
        sin((float)(M_PI * i * BEEP_FREQUENCY) / (float)SAMPLING_FREQUENCY);
    sound_buffer[i] = (short)(audio_value * LOUDNESS);
  }

  SDL_PutAudioStreamData(stream, sound_buffer,
                         SAMPLING_FREQUENCY * sizeof(short));
  free(sound_buffer);
}

int main(int argc, char **argv) {
  SDL_Window *window;

  /* Initialize SDL */
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

  SDL_AudioDeviceID audio_id =
      SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (audio_id == 0) {
    printf("Failed to open audio: %s\n", SDL_GetError());
    return 1;
  }

  /* Create main audio stream */
  SDL_AudioSpec src_spec = {SDL_AUDIO_S16, 2, SAMPLING_FREQUENCY};
  SDL_AudioSpec dst_spec = {SDL_AUDIO_S16, 2, SAMPLING_FREQUENCY};
  SDL_AudioStream *stream = SDL_CreateAudioStream(&src_spec, &dst_spec);
  if (!SDL_BindAudioStream(audio_id, stream)) {
    printf("Failed to bind audio: %s\n", SDL_GetError());
    return 1;
  }

  /* Pause audio device before beginning */
  if (!SDL_PauseAudioStreamDevice(stream)) {
    printf("Failed to pause audio device: %s\n", SDL_GetError());
  }

  add_sinusoidal_audio(stream);

  /* Configure chip-8 components */
  unsigned char *memory = (unsigned char *)malloc(CHIP8_MEMORY);
  chip8_register *registers = (chip8_register *)malloc(sizeof(chip8_register));
  unsigned short *stack =
      (unsigned short *)malloc(CHIP8_STACK * sizeof(unsigned short));

  /* Display */
  unsigned char *display =
      (unsigned char *)malloc(CHIP8_WIDTH * CHIP8_HEIGHT / 8);
  memset(display, 0, CHIP8_WIDTH * CHIP8_HEIGHT / 8);

  setup_graphics(memory);

  /* Read in chip8 ROM file */
  // char *filename = "roms/games/Cave.ch8";
  char *filename = tinyfd_openFileDialog("Open a file", "", 0, NULL, NULL, 0);
  printf("Playing file: %s\n", filename);
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    printf("Read error\n");
    return 1;
  }

  /* Load program in memory */
  unsigned char *pointer = memory + 0x200;
  unsigned char read_buf;
  while (read(fd, &read_buf, 1) == 1) {
    *(pointer++) = read_buf;
  }

  /* PC point to start of program */
  registers->PC = 0x0200;

  /* Create window */
  window = SDL_CreateWindow("Chip-8 Emulator", CHIP8_WIDTH * 10,
                            CHIP8_HEIGHT * 10, SDL_WINDOW_OPENGL);
  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  /* Create renderer */
  SDL_Renderer *renderer = SDL_CreateRenderer(window, "opengl");

  /* Create bitmap texture for chip-8 */
  SDL_Texture *texture_on = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                              SDL_TEXTUREACCESS_STATIC, 10, 10);

  SDL_Texture *texture_off = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 10, 10);

  void *on_buffer = malloc(100 * sizeof(uint32_t));
  void *off_buffer = malloc(100 * sizeof(uint32_t));
  memset(on_buffer, 0xFF, 100 * sizeof(uint32_t));
  memset(off_buffer, 0x00, 100 * sizeof(uint32_t));

  SDL_UpdateTexture(texture_on, NULL, on_buffer, 10 * sizeof(uint32_t));
  SDL_UpdateTexture(texture_off, NULL, off_buffer, 10 * sizeof(uint32_t));

  uint64_t frequency = SDL_GetPerformanceFrequency();
  uint64_t cycle_duration = frequency / 500; /* 500Hz clock speed */
  uint64_t frame_duration = frequency / 60;  /* 60Hz frame display */

  uint64_t next_cycle_time = SDL_GetPerformanceCounter();
  uint64_t next_frame_time = next_cycle_time;

  /* Main loop */
  SDL_Event event;
  const bool *keys = SDL_GetKeyboardState(NULL);
  // dump_state(memory, registers, stack, display);
  while (1) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        break;
      }
    }

    int audio_left = SDL_GetAudioStreamAvailable(stream);
    if (audio_left < SAMPLING_FREQUENCY / 10) {
      add_sinusoidal_audio(stream);
    }

    uint64_t current_time = SDL_GetPerformanceCounter();

    if (current_time > next_cycle_time) {
      unsigned short instruction =
          ((unsigned short)memory[registers->PC] << 8) |
          memory[registers->PC + 1];
      registers->PC += 2;
      handle_instruction(instruction, memory, registers, stack, display, keys,
                         stream, &event);
      next_cycle_time += cycle_duration;
      // dump_state(memory, registers, stack, display);
    }

    if (current_time > next_frame_time) {
      SDL_RenderClear(renderer);
      for (int y = 0; y < CHIP8_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_WIDTH; x++) {
          SDL_Texture *texture =
              get_pixel(x, y, display) ? texture_on : texture_off;
          SDL_FRect frect = {x * 10, y * 10, 10, 10};
          SDL_RenderTexture(renderer, texture, NULL, &frect);
        }
      }

      SDL_RenderPresent(renderer);
      next_frame_time += frame_duration;

      /* Decrement delay & sound timer */
      if (registers->delay_timer > 0)
        registers->delay_timer--;
      if (registers->sound_timer > 0) {
        SDL_ResumeAudioStreamDevice(stream);
        registers->sound_timer--;
      } else {
        SDL_PauseAudioStreamDevice(stream);
      }
    }

    SDL_Delay(1);
  }

  /* Unbind audio stream */
  SDL_UnbindAudioStream(stream);

  /* Destroy audio stream */
  SDL_DestroyAudioStream(stream);

  /* Free Audio Device */
  SDL_CloseAudioDevice(audio_id);

  /* Destroy window */
  SDL_DestroyTexture(texture_on);
  SDL_DestroyTexture(texture_off);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  /* Free memory */
  free(memory);
  free(registers);
  free(stack);
  free(display);

  /* Quit SDL */
  SDL_Quit();
  return 0;
}
