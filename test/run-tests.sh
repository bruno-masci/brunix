#!/bin/bash

set -e
set -o pipefail
rm -f multiboot-test; gcc -D__KERNEL_H__ -m32 -o multiboot-test multiboot-test.c ../arch/x86/kernel/multiboot.c -I/home/osdev/workspace/brunix/include -I/home/osdev/workspace/brunix/arch/x86/include -lcunit; ./multiboot-test
rm -f gdt-test; gcc -m32 -o gdt-test gdt-test.c ../arch/x86/kernel/gdt.c -I/home/osdev/workspace/brunix/include -I/home/osdev/workspace/brunix/arch/x86/include -lcunit; ./gdt-test
rm -f vm-test; gcc -m32 -Dprintk=printf -o vm-test vm-test.c ../kernel/vm.c -I/home/osdev/workspace/brunix/include -I/home/osdev/workspace/brunix/arch/x86/include -lcunit; ./vm-test