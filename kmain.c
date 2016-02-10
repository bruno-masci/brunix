				 #include "io.h"

				/* The I/O ports */
				#define FB_COMMAND_PORT         0x3D4
				#define FB_DATA_PORT            0x3D5

				/* The I/O port commands */
				#define FB_HIGH_BYTE_COMMAND    14
				#define FB_LOW_BYTE_COMMAND     15

				/** fb_move_cursor:
				 *  Moves the cursor of the framebuffer to the given position
				 *
				 *  @param pos The new position of the cursor
				 */
				void fb_move_cursor(unsigned short pos)
				{
				    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
				    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
				    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
				    outb(FB_DATA_PORT,    pos & 0x00FF);
				}



//ver mas en http://www.osdever.net/bkerndev/Docs/printing.htm
				//http://wiki.osdev.org/Printing_To_Screen


/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */




void fb_write_cell(unsigned int i, unsigned char c, unsigned char fg, unsigned char bg) {
	char *fb = (char *) 0x000B8000;

//clear
	#define WHITE_TXT 0x07 // white on black text
	/*unsigned int j=0;
	while(j < (80*25*2))
	{
		fb[j]=' ';
		j++;
		fb[j]=WHITE_TXT;
		j++;
	};*/


    int j=0;
    unsigned char *str = "hola pepe!";
    while (*str != '\0') {
    	
    	fb[j] = *str;
    	j+=2;
    	//fb[j++] = (1 << 8);//((fg & 0x0F) << 4) | (bg & 0x0F);
    	
    	str++;
	    
	}
	fb[0] = 'a';
	fb[3] = 'b';
	fb[6] = 'c';
	fb[9] = 'd';

	unsigned short *where = (unsigned short *)fb + 323;
	*where = 'a' | (1 << 8);
}

#define FB_GREEN     2
#define FB_DARK_GREY 8

unsigned int k_printf() // the message and then the line #
{
	int line = 0;
	char *message = "Holas!!\nVideo is working :)";
	char *vidmem = (char *) 0x000B8000;
	unsigned int i=0;

	i=(line*80*2);

	while(*message!=0)
	{
		if(*message=='\n') // check for a new line
		{
			line++;
			i=(line*80*2);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=WHITE_TXT;
			i++;
		};
	};

	return(1);
};



int kmain() {
	int i=1;
	//while (i< 10000) {
		//fb_write_cell(0, 65, FB_GREEN, FB_DARK_GREY);
		//i++;
	//}
	
	
		//fb_move_cursor(0);
	
	//k_printf();

	//for (;;);


/*
*((int*)0xb8000)=0x07690748;

*((char*)0xb8000)=0x48;
*((char*)0xb8001)=0x07;
*((char*)0xb8002)=0x48;
*((char*)0xb8003)=0x07;*/

write_string( 0x1F, "aloha");
	return 0;
}

// note this example will always write to the top
// line of the screen
void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    char c = 0x48;
    while( *string != 0 )
    {
        *video = c++;
        video++;
		*video=0x07;
		video++;
		string++;
    }
}

/*
qemu-system-x86_64 -cdrom os.iso -S -s & gdb init.elf \
        -ex 'target remote localhost:1234' \
        -ex 'layout src' \
        -ex 'layout regs' \
        -ex 'layout asm' \
        -ex 'break multiboot_entry' \
        -ex 'continue'
        */


/*        otra opcion para io.s:

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}*/