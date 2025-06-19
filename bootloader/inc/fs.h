#ifndef FS_H
#define FS_H

#include <stdint.h>

void load_directory();
void command_ls();
void command_cat(const char* filename);
void command_write(const char* filename, const char* content);
void command_mkdir(const char* dirname);
void command_rm(const char* filename);
void command_cd(const char* dirname);
void command_pwd();

#endif
