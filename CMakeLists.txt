cmake_minimum_required(VERSION 3.16)
project(chip8)

set(EXECUTABLE_NAME ${PROJECT_NAME})

# Add SDL Library
# This add_subdirectory command builds the SDL library according to CMakeLists.txt in the SDL source code
# According to the that CMakeLists.txt, the SDL library is built as a library named SDL3::SDL3
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

# Add the executable
add_executable(${EXECUTABLE_NAME} src/main.c src/graphics.c src/utils.c src/tinyfiledialogs.c)

# Include the header files
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Link the SDL library
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL3::SDL3)
