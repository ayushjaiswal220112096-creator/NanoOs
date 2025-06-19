#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

void clear_screen();
void print(const char* str);
void print_char(char ch);
void update_cursor();
void outb(uint16_t port, uint8_t val);

#endif
