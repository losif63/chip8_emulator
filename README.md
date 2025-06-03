# CHIP8 EMULATOR

This is a simple chip8 emulator built in C.
The code has been primarily devloped & tested on MacOSX.

## Dependencies
- SDL3 (Rendering / Audio)
- CMake (Build)

## TODO
- [x] Add sound support
- [ ] Add installation code
- [x] Add instructions for how to load chip8 files
- [ ] Test & add Linux support
- [ ] Test & add Windows support

## How to build

Execute the following commands to build and run the binary executable.
```
mkdir build
cd build
cmake ..
cmake --build .
cd ..
./build/chip8
```
