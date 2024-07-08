cmake_minimum_required(VERSION 3.25)
project(ImageToAscii VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(C_STANDARD 23)

set(SOURCES src/main.c src/framebuffer.c src/framebuffer.h include/stb_image.h)
add_executable(ImageToAscii ${SOURCES})
target_include_directories(ImageToAscii PRIVATE include)
#target_compile_options(ImageToAscii PRIVATE -Zi)

