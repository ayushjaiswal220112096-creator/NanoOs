#include "string.h"
#include <stddef.h>

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, unsigned int n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1; ++s2; --n;
    }
    return n == 0 ? 0 : *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void* memcpy(void* dest, const void* src, unsigned int count) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    while (count--) *d++ = *s++;
    return dest;
}

void* memset(void* dest, int val, unsigned int count) {
    unsigned char* ptr = dest;
    while (count--) *ptr++ = (unsigned char)val;
    return dest;
}

char* strchr(const char* str, char ch) {
    while (*str && *str != ch) {
        str++;
    }
    return (*str == ch) ? (char*)str : NULL;
}

char* strcat(char* dest, const char* src) {
    char* original_dest = dest;
    
    // Find the end of the destination string
    while (*dest) {
        dest++;
    }
    
    // Copy the source string
    while (*src) {
        *dest++ = *src++;
    }
    
    // Add null terminator
    *dest = '\0';
    
    return original_dest;
}

char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    
    while (*src) {
        *dest++ = *src++;
    }
    
    *dest = '\0';
    
    return original_dest;
}
