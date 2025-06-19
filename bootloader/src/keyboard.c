#include "keyboard.h"
#include "screen.h"

#define BACKSPACE 0x0E
#define ENTER     0x1C

char scancode_to_char[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',  // Backspace (0x0E)
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // Enter key
    0,   'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',
    0,   '*',
    0,  ' '  // Space bar
    // Add more keys if needed
};

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void wait_for_key_release() {
    unsigned char scancode;
    do {
        scancode = inb(0x60);
    } while (!(scancode & 0x80)); // Wait until key is released (bit 7 set)
}

static inline char keyboard_read_char() {
    unsigned char scancode = 0;
    
    // Wait for a key press
    while (1) {
        scancode = inb(0x60);
        
        // Check if it's a key press (not a key release)
        if (!(scancode & 0x80)) {
            // If valid scancode and has a character mapping
            if (scancode < 128 && scancode_to_char[scancode] != 0) {
                // Wait for key release to prevent repeats
                wait_for_key_release();
                return scancode_to_char[scancode];
            }
        }
    }
}


void read_input(char *buffer) {
    int i = 0;
    while (1) {
        char c = keyboard_read_char();

        if (c == '\n') {
            buffer[i] = '\0';
            print_char('\n');
            return;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                print_char('\b');
            }
        } else {
            buffer[i++] = c;
            print_char(c);
        }
    }
}
