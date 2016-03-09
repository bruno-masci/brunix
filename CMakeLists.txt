cmake_minimum_required(VERSION 3.3)
project(brunix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    arch/x86/include/asm/gdt.h
    arch/x86/kernel/vga.c
    arch/x86/kernel/vga.h
    include/brunix/stddef.h
    include/stdint.h
    include/stdlib.h
    include/string.h
    iso/boot/grub/grub.cfg
    libc/stdlib.c
    libc/string.c
    bochsrc.txt
    boot.s
    CMakeLists.txt
    descriptor_tables.c
    descriptor_tables.h
    gdt.s
    interrupt.s
    io.c
    io.h
    isr.c
    isr.h
    kmain.c
    linker.ld
    Makefile
    multiboot_header.s
    README.md)

add_executable(brunix ${SOURCE_FILES})