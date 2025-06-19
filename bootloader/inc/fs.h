#ifndef FS_H
#define FS_H

#include <stdint.h>

void load_directory();
void command_ls();
void command_cat(const char* filename);

#endif