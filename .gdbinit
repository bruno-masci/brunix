set architecture i386

echo + target remote localhost:26000\n
target remote localhost:26000

echo + symbol-file brunix.elf\n
symbol-file brunix.elf
file brunix.elf

layout asm
layout regs
#break kmain
