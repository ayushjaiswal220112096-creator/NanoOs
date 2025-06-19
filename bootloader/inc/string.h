#ifndef STRING_H
#define STRING_H

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, unsigned int n);
int strlen(const char* str);
void* memcpy(void* dest, const void* src, unsigned int count);
void* memset(void* dest, int val, unsigned int count);

#endif
