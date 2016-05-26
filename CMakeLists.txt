cmake_minimum_required(VERSION 3.3)
project(brunix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    arch/x86/include/asm/gdt.h
    arch/x86/include/asm/isr.h
    arch/x86/kernel/gdt.c
    arch/x86/kernel/isr.c
    arch/x86/kernel/vga.c
    arch/x86/include/asm/vga.h
    arch/x86/include/asm/io.h
    arch/x86/include/asm/stddef.h
    include/brunix/stdlib.h
    include/brunix/string.h
    iso/boot/grub/grub.cfg
    libkern/stdlib.c
    libkern/string.c
    bochsrc.txt
    CMakeLists.txt
    kernel/main.c
    linker.ld
    Makefile
    README.md)

add_executable(brunix ${SOURCE_FILES})