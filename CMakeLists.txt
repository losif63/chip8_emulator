cmake_minimum_required(VERSION 3.16)
project(chip8_emulator)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create your game executable target as usual
add_executable(chip8 src/main.c src/utils.c)

# Link to the actual SDL3 library.
target_link_libraries(chip8 PRIVATE SDL3::SDL3)