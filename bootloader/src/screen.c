#include <stdint.h>
#include "screen.h"

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

uint16_t* video_memory = (uint16_t*) VIDEO_ADDRESS;
int cursor_offset = 0;

void clear_screen() {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_offset = 0;
    update_cursor();
}

void update_cursor() {
    // Send the high byte
    outb(0x3D4, 14);
    outb(0x3D5, (cursor_offset >> 8) & 0xFF);
    // Send the low byte
    outb(0x3D4, 15);
    outb(0x3D5, cursor_offset & 0xFF);
}

void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void print_char(char ch) {
    if (ch == '\n') {
        // Move to the start of the next line
        cursor_offset = (cursor_offset / MAX_COLS + 1) * MAX_COLS;
    } 
    else if (ch == '\b') {
        // Handle backspace - move back one character and clear it
        if (cursor_offset > 0) {
            cursor_offset--;
            video_memory[cursor_offset] = (WHITE_ON_BLACK << 8) | ' ';
        }
    }
    else {
        // Print the character at the current cursor position
        video_memory[cursor_offset] = (WHITE_ON_BLACK << 8) | ch;
        cursor_offset++;
    }

    // Scroll if needed
    if (cursor_offset >= MAX_ROWS * MAX_COLS) {
        // Scroll up by one line
        for (int i = 0; i < (MAX_ROWS-1) * MAX_COLS; i++) {
            video_memory[i] = video_memory[i + MAX_COLS];
        }
        
        // Clear the last line
        for (int i = (MAX_ROWS-1) * MAX_COLS; i < MAX_ROWS * MAX_COLS; i++) {
            video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
        }
        
        // Move cursor to the beginning of the last line
        cursor_offset = (MAX_ROWS-1) * MAX_COLS;
    }
    
    // Update the hardware cursor
    update_cursor();
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}
