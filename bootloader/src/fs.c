
#include "fs.h"
#include "screen.h"
#include <stdint.h>
#include "string.h"

#define MAX_FILES 16
#define DIR_MEMORY_PTR ((uint8_t*)0x90000)

struct file_entry {
    char name[12];
    uint32_t start;
    uint32_t size;
};

struct file_entry files[MAX_FILES];

// Load directory from memory (populated by bootloader)
void load_directory() {
    struct file_entry *dir = (struct file_entry *)DIR_MEMORY_PTR;
    for (int i = 0; i < MAX_FILES; i++) {
        files[i] = dir[i];
    }
}

void command_ls() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0') {
            print(files[i].name);
            print("\n");
        }
    }
}

void command_cat(const char* filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            // Calculate file data location based on start sector
            // Assuming file data starts at physical address 0x10000
            uint8_t* file_data = (uint8_t*)(0x10000 + (files[i].start - 21) * 512);
            for (uint32_t j = 0; j < files[i].size; j++) {
                print_char(file_data[j]);
            }
            print_char('\n');
            return;
        }
    }
    print("File not found\n");
}
