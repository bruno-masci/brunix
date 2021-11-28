#!/bin/bash

set -e
set -o pipefail
rm -f multiboot-test; gcc -D__STRING_H2__ -D__KERNEL_H__ -m32 -o multiboot-test multiboot-test.c ../kernel/multiboot.c -I/home/osdev/workspace/brunix/include -lcunit; ./multiboot-test
rm -f gdt-test; gcc -m32 -o gdt-test gdt-test.c ../arch/x86/kernel/gdt.c -I/home/osdev/workspace/brunix/include -lcunit; ./gdt-test
#FIXME rm -f vm-test; gcc -m32 -o vm-test vm-test.c ../kernel/vm.c -I/home/osdev/workspace/brunix/include -lcunit; ./vm-test