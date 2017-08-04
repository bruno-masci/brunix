cmake_minimum_required(VERSION 3.3)
project(brunix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        include/arch/x86/vga.h
        include/brunix/stdlib.h
        iso/boot/grub/grub.cfg
    libkern/stdlib.c
        bochsrc.txt
    CMakeLists.txt
    kernel/main.c
    linker.ld
    Makefile
    README.md)

add_executable(brunix ${SOURCE_FILES})