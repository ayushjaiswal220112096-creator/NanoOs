#ifndef STRING_H
#define STRING_H

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, unsigned int n);
int strlen(const char* str);
void* memcpy(void* dest, const void* src, unsigned int count);
void* memset(void* dest, int val, unsigned int count);
char* strchr(const char* str, char ch);
char* strrchr(const char* str, char ch);
char* strcat(char* dest, const char* src);
char* strcpy(char* dest, const char* src);

#endif
