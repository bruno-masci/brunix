// From http://www.osdever.net/bkerndev/Docs/keyboard.htm

#include <asm/isr.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <brunix/kernel.h>


#define KEYBOARD_IRQ 1


//
static char buf[1024];
static int buf_len = -1; //FIXME si le pongo 0, se comporta muy extranio

/* KBDUS means US Keyboard Layout. This is a scancode table
 * used to layout a standard US keyboard. I have left some
 * comments in to give you an idea of what key is what, even
 * though I set it's array index to 0. You can change that to
 * whatever you want using a macro, if you wish! */
static unsigned char kbd_US[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	    /* 9 */
        '9', '0', '-', '=', '\b',	                        /* Backspace */
        '\t',			                                    /* Tab */
        'q', 'w', 'e', 'r',	                                /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	    /* Enter key */
        0,			                                        /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
        '\'', '`',   0,		                                /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',			        /* 49 */
        'm', ',', '.', '/',   0,				            /* Right shift */
        '*',
        0,	    /* Alt */
        ' ',	/* Space bar */
        0,	    /* Caps lock */
        0,	    /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,	    /* < ... F10 */
        0,	    /* 69 - Num lock*/
        0,	    /* Scroll Lock */
        0,	    /* Home key */
        0,	    /* Up Arrow */
        0,	    /* Page Up */
        '-',
        0,	    /* Left Arrow */
        0,
        0,	    /* Right Arrow */
        '+',
        0,	/* 79 - End key*/
        0,	/* Down Arrow */
        0,	/* Page Down */
        0,	/* Insert Key */
        0,	/* Delete Key */
        0,   0,   0,
        0,	/* F11 Key */
        0,	/* F12 Key */
        0,	/* All other keys are undefined */
};

void handle_command(char *s) {
    printk("HOLIS - %s-\n", s);
}

static void kbd_callback(__attribute__((unused)) struct registers_t *regs) {
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80) {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    } else {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
        char character = kbd_US[scancode];
        printk("%c", character);
        if (character == '\n') {
            buf_len = buf_len == -1 ? 0 : buf_len;//FIXME ver arriba
            buf[buf_len] = '\0';
//            printk("BUFFER = %s, LEN = %d\n", buf, buf_len);
            handle_command(buf);
            buf_len = 0;
            printk(">> ");
        } else {
            buf[buf_len++] = character;
            buf[buf_len] = '\0';
        }
    }
}

void kbd_init(void) {
    printk("KEYB INIT\n");
    memset(buf, 0, 1024);
    request_irq(KEYBOARD_IRQ, kbd_callback);

//    request_irq(KEYBOARD_IRQ, keyboard_interrupt, 0, "keyboard");
//    request_region(0x60,1,"kbd");
//    request_region(0x64,1,"kbd");
}

//unsigned long kbd_init(unsigned long kmem_start)
//{
//    int i;
//    struct kbd_struct kbd0;
//    extern struct tty_driver console_driver;
//
//    kbd0.ledflagstate = kbd0.default_ledflagstate = KBD_DEFLEDS;
//    kbd0.ledmode = LED_SHOW_FLAGS;
//    kbd0.lockstate = KBD_DEFLOCK;
//    kbd0.modeflags = KBD_DEFMODE;
//    kbd0.kbdmode = VC_XLATE;
//
//    for (i = 0 ; i < MAX_NR_CONSOLES ; i++)
//        kbd_table[i] = kbd0;
//
//    ttytab = console_driver.table;
//
//    bh_base[KEYBOARD_BH].routine = kbd_bh;
//    request_irq(KEYBOARD_IRQ, keyboard_interrupt, 0, "keyboard");
//    request_region(0x60,1,"kbd");
//    request_region(0x64,1,"kbd");
//    mark_bh(KEYBOARD_BH);
//    enable_bh(KEYBOARD_BH);
//    return kmem_start;
//}
