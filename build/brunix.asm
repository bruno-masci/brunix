
brunix.elf:     file format elf64-x86-64


Disassembly of section .boot:

0000000000100000 <.boot>:
  100000:	02 b0 ad 1b 00 00    	add    0x1bad(%rax),%dh
  100006:	00 00                	add    %al,(%rax)
  100008:	fe 4f 52             	decb   0x52(%rdi)
  10000b:	e4                   	.byte 0xe4

Disassembly of section .text:

0000000000101000 <_start+0xff4>:
  101000:	eb 00                	jmp    101002 <.hang>

0000000000101002 <.hang>:
.hang():
  101002:	f4                   	hlt    
  101003:	eb fd                	jmp    101002 <.hang>

Disassembly of section .interp:

0000000000102000 <.interp>:
  102000:	2f                   	(bad)  
  102001:	6c                   	insb   (%dx),%es:(%rdi)
  102002:	69 62 36 34 2f 6c 64 	imul   $0x646c2f34,0x36(%rdx),%esp
  102009:	2d 6c 69 6e 75       	sub    $0x756e696c,%eax
  10200e:	78 2d                	js     10203d <.end+0x1038>
  102010:	78 38                	js     10204a <.end+0x1045>
  102012:	36 2d 36 34 2e 73    	ss sub $0x732e3436,%eax
  102018:	6f                   	outsl  %ds:(%rsi),(%dx)
  102019:	2e 32 00             	xor    %cs:(%rax),%al

Disassembly of section .note.gnu.build-id:

000000000010201c <.note.gnu.build-id>:
  10201c:	04 00                	add    $0x0,%al
  10201e:	00 00                	add    %al,(%rax)
  102020:	14 00                	adc    $0x0,%al
  102022:	00 00                	add    %al,(%rax)
  102024:	03 00                	add    (%rax),%eax
  102026:	00 00                	add    %al,(%rax)
  102028:	47                   	rex.RXB
  102029:	4e 55                	rex.WRX push %rbp
  10202b:	00 8d 46 6f 97 17    	add    %cl,0x17976f46(%rbp)
  102031:	cd 7f                	int    $0x7f
  102033:	bd c1 d9 96 21       	mov    $0x2196d9c1,%ebp
  102038:	d0 14 c6             	rclb   (%rsi,%rax,8)
  10203b:	4b f1                	rex.WXB icebp 
  10203d:	75 aa                	jne    101fe9 <.end+0xfe4>
  10203f:	81                   	.byte 0x81

Disassembly of section .dynsym:

0000000000102040 <.dynsym>:
	...

Disassembly of section .dynstr:

0000000000102058 <.dynstr>:
	...

Disassembly of section .gnu.hash:

0000000000102060 <.gnu.hash>:
  102060:	01 00                	add    %eax,(%rax)
  102062:	00 00                	add    %al,(%rax)
  102064:	01 00                	add    %eax,(%rax)
  102066:	00 00                	add    %al,(%rax)
  102068:	01 00                	add    %eax,(%rax)
	...

Disassembly of section .dynamic:

0000000000102080 <_DYNAMIC>:
  102080:	f5                   	cmc    
  102081:	fe                   	(bad)  
  102082:	ff 6f 00             	ljmp   *0x0(%rdi)
  102085:	00 00                	add    %al,(%rax)
  102087:	00 60 20             	add    %ah,0x20(%rax)
  10208a:	10 00                	adc    %al,(%rax)
  10208c:	00 00                	add    %al,(%rax)
  10208e:	00 00                	add    %al,(%rax)
  102090:	05 00 00 00 00       	add    $0x0,%eax
  102095:	00 00                	add    %al,(%rax)
  102097:	00 58 20             	add    %bl,0x20(%rax)
  10209a:	10 00                	adc    %al,(%rax)
  10209c:	00 00                	add    %al,(%rax)
  10209e:	00 00                	add    %al,(%rax)
  1020a0:	06                   	(bad)  
  1020a1:	00 00                	add    %al,(%rax)
  1020a3:	00 00                	add    %al,(%rax)
  1020a5:	00 00                	add    %al,(%rax)
  1020a7:	00 40 20             	add    %al,0x20(%rax)
  1020aa:	10 00                	adc    %al,(%rax)
  1020ac:	00 00                	add    %al,(%rax)
  1020ae:	00 00                	add    %al,(%rax)
  1020b0:	0a 00                	or     (%rax),%al
  1020b2:	00 00                	add    %al,(%rax)
  1020b4:	00 00                	add    %al,(%rax)
  1020b6:	00 00                	add    %al,(%rax)
  1020b8:	01 00                	add    %eax,(%rax)
  1020ba:	00 00                	add    %al,(%rax)
  1020bc:	00 00                	add    %al,(%rax)
  1020be:	00 00                	add    %al,(%rax)
  1020c0:	0b 00                	or     (%rax),%eax
  1020c2:	00 00                	add    %al,(%rax)
  1020c4:	00 00                	add    %al,(%rax)
  1020c6:	00 00                	add    %al,(%rax)
  1020c8:	18 00                	sbb    %al,(%rax)
  1020ca:	00 00                	add    %al,(%rax)
  1020cc:	00 00                	add    %al,(%rax)
  1020ce:	00 00                	add    %al,(%rax)
  1020d0:	15 00 00 00 00       	adc    $0x0,%eax
	...
  1020dd:	00 00                	add    %al,(%rax)
  1020df:	00 1e                	add    %bl,(%rsi)
  1020e1:	00 00                	add    %al,(%rax)
  1020e3:	00 00                	add    %al,(%rax)
  1020e5:	00 00                	add    %al,(%rax)
  1020e7:	00 08                	add    %cl,(%rax)
  1020e9:	00 00                	add    %al,(%rax)
  1020eb:	00 00                	add    %al,(%rax)
  1020ed:	00 00                	add    %al,(%rax)
  1020ef:	00 fb                	add    %bh,%bl
  1020f1:	ff                   	(bad)  
  1020f2:	ff 6f 00             	ljmp   *0x0(%rdi)
  1020f5:	00 00                	add    %al,(%rax)
  1020f7:	00 01                	add    %al,(%rcx)
  1020f9:	00 00                	add    %al,(%rax)
  1020fb:	08 00                	or     %al,(%rax)
	...
