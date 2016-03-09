# brunix
Small x86 OS for fun and learning (in progress)

The state the computer gets put into by GrUB is as follows:
Protected mode
A20 Gate enabled
EBX contains a pointer to the Multiboot information structure
EAX contains the value 0x2BADB002
Paging off
Stack somewhere in memory
Interrupts disabled
Ver http://www.codeproject.com/Articles/39027/Beginning-Operating-System-Development-Part-One




References:

http://www.jamesmolloy.co.uk/tutorial_html

http://wiki.osdev.org

http://os.phil-opp.com/multiboot-kernel.html

http://www.osdever.net/bkerndev/index.php

http://www.jofre.de/?download=WritingAnOs.pdf

