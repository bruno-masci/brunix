/**
 * @author Bruno Masci
 * @brief VGA output related code
 *
 * Heavily based on Bran's kernel development tutorials, but rewritten for JamesM's kernel tutorials.
 * TODO ver vga.c de eduOS a ver si se puede mejorar algo.
 */

#include "../include/arch/x86/vga.h"
#include "../include/arch/x86/io.h"

// The VGA framebuffer starts at 0xB8000.
volatile uint16_t *video_memory = (volatile uint16_t *)VIDEO_MEM_ADDR;
// Stores the cursor position.
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

uint8_t backColour = COLOR_BLACK;
uint8_t foreColour = COLOR_WHITE;


void vga_set_foreground_color(uint8_t colour) {
	foreColour = colour;
}



/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15


// Updates the hardware cursor.
static void move_cursor() {
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;		// The screen is 80 characters wide...
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);			// Tell the VGA board we are setting the high cursor byte.
    outb(FB_DATA_PORT, cursorLocation >> 8);				// Send the high cursor byte.
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);				// Tell the VGA board we are setting the low cursor byte.
    outb(FB_DATA_PORT, cursorLocation);						// Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll() {
    // Get a space character with the default colour attributes.
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

void vga_init() {
    vga_clear();
}

// Writes a single character out to the screen.
void vga_putc(char c) {
    // The background colour is black (0), the foreground is white (15).

    // The attribute byte is made up of two nibbles - the lower being the
    // foreground colour, and the upper the background colour.
    uint8_t  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    uint16_t attribute = attributeByte << 8;
    volatile uint16_t *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x) {
        cursor_x--;
    }

        // Handle a tab by increasing the cursor's X, but only to a point
        // where it is divisible by 8.
    else if (c == 0x09) {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

        // Handle carriage return
    else if (c == '\r') {
        cursor_x = 0;
    }

        // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    }
        // Handle any other printable character.
    else if(c >= ' ') {
        location = video_memory + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
}


// Clears the screen, by copying lots of spaces to the framebuffer.
void vga_clear() {
    // Make an attribute byte for the default colours
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++) {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void vga_puts(char *str) {
    for (int i = 0; str[i]; i++) {
        vga_putc(str[i]);
    }
}

static void vga_write_number(uint32_t n, int base) {
    char str[64+1];
    itoa(n, str, base);
    vga_puts(str);
}

void vga_puthex(uint32_t n) {
    vga_puts("0x");
    vga_write_number(n, 16);
}

void vga_putdec(uint32_t n) {
    vga_write_number(n, 10);
}

void vga_putbin(uint32_t n) {
    vga_write_number(n, 2);
}
