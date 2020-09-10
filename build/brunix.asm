
brunix.elf:     file format elf32-i386


Disassembly of section .boot:

00100000 <.boot>:
  100000:	02 b0 ad 1b 00 00    	add    0x1bad(%eax),%dh
  100006:	00 00                	add    %al,(%eax)
  100008:	fe 4f 52             	decb   0x52(%edi)
  10000b:	e4                   	.byte 0xe4

Disassembly of section .text:

00101000 <_start+0xff4>:
  101000:	eb 00                	jmp    101002 <.hang>

00101002 <.hang>:
.hang():
  101002:	f4                   	hlt    
  101003:	eb fd                	jmp    101002 <.hang>
