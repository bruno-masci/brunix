cmake_minimum_required(VERSION 3.3)
project(brunix)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    iso/boot/grub/grub.cfg
    bochsout.log
    bochsrc.txt
    boot.s
    common.c
    common.h
    gdt.c
    io.h
    io.s
    kmain.c
    linker.ld
    Makefile
    monitor.c
    monitor.h
    multiboot_header.s
    README.md)

add_executable(brunix ${SOURCE_FILES})