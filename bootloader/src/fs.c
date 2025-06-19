
#include "fs.h"
#include "screen.h"
#include <stdint.h>
#include "string.h"
#include <stddef.h>

#define MAX_FILES 16
#define DIR_MEMORY_PTR ((uint8_t*)0x90000)
#define FILE_DATA_START 0x10000
#define SECTOR_SIZE 512
#define MAX_FILENAME_LENGTH 12
#define MAX_CONTENT_LENGTH 1024
#define FILE_TYPE 0
#define DIR_TYPE 1

struct file_entry {
    char name[MAX_FILENAME_LENGTH];
    uint32_t start;
    uint32_t size;
    uint8_t type;  // 0 = file, 1 = directory
    uint32_t parent_dir;  // Index of parent directory
};

struct file_entry files[MAX_FILES];
uint32_t current_dir = 0;  // Root directory
char current_path[256] = "/";

// Load directory from memory (populated by bootloader)
void load_directory() {
    struct file_entry *dir = (struct file_entry *)DIR_MEMORY_PTR;
    for (int i = 0; i < MAX_FILES; i++) {
        files[i] = dir[i];
        // Set type to file by default for existing entries
        if (files[i].name[0] != '\0') {
            files[i].type = FILE_TYPE;
            files[i].parent_dir = 0;  // Root directory
        }
    }
    
    // Create root directory if it doesn't exist
    if (files[0].name[0] == '\0') {
        strcpy(files[0].name, "/");
        files[0].start = 0;
        files[0].size = 0;
        files[0].type = DIR_TYPE;
        files[0].parent_dir = 0;  // Root is its own parent
    }
}

void command_ls() {
    print("Contents of ");
    print(current_path);
    print(":\n");
    
    int found = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0' && files[i].parent_dir == current_dir) {
            if (files[i].type == DIR_TYPE) {
                print("[DIR] ");
            } else {
                print("[FILE] ");
            }
            print(files[i].name);
            print("\n");
            found = 1;
        }
    }
    
    if (!found) {
        print("Directory is empty\n");
    }
}

void command_cat(const char* filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, filename) == 0 && files[i].parent_dir == current_dir) {
            if (files[i].type == DIR_TYPE) {
                print("Error: Cannot cat a directory\n");
                return;
            }
            
            // Calculate file data location based on start sector
            uint8_t* file_data = (uint8_t*)(FILE_DATA_START + (files[i].start - 21) * SECTOR_SIZE);
            for (uint32_t j = 0; j < files[i].size; j++) {
                print_char(file_data[j]);
            }
            print_char('\n');
            return;
        }
    }
    print("File not found\n");
}

void command_write(const char* filename, const char* content) {
    int file_index = -1;
    int content_length = strlen(content);
    
    // Check if file already exists
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, filename) == 0 && files[i].parent_dir == current_dir) {
            file_index = i;
            break;
        }
    }
    
    // If file doesn't exist, find an empty slot
    if (file_index == -1) {
        for (int i = 0; i < MAX_FILES; i++) {
            if (files[i].name[0] == '\0') {
                file_index = i;
                break;
            }
        }
    }
    
    if (file_index == -1) {
        print("Error: No space left in file system\n");
        return;
    }
    
    if (content_length > MAX_CONTENT_LENGTH) {
        print("Error: Content too large\n");
        return;
    }
    
    // Find a free sector for the file data
    uint32_t sector = 21;  // Start from sector 21
    if (files[file_index].start != 0) {
        sector = files[file_index].start;  // Use existing sector if file exists
    } else {
        // Find a free sector
        int used_sectors[MAX_FILES] = {0};
        for (int i = 0; i < MAX_FILES; i++) {
            if (files[i].name[0] != '\0' && files[i].start != 0) {
                used_sectors[files[i].start - 21] = 1;
            }
        }
        
        for (int i = 0; i < MAX_FILES; i++) {
            if (used_sectors[i] == 0) {
                sector = 21 + i;
                break;
            }
        }
    }
    
    // Update file entry
    strcpy(files[file_index].name, filename);
    files[file_index].start = sector;
    files[file_index].size = content_length;
    files[file_index].type = FILE_TYPE;
    files[file_index].parent_dir = current_dir;
    
    // Write file content
    uint8_t* file_data = (uint8_t*)(FILE_DATA_START + (sector - 21) * SECTOR_SIZE);
    for (int i = 0; i < content_length; i++) {
        file_data[i] = content[i];
    }
    
    print("File written successfully\n");
}

void command_mkdir(const char* dirname) {
    if (strlen(dirname) >= MAX_FILENAME_LENGTH) {
        print("Error: Directory name too long\n");
        return;
    }
    
    // Check if directory already exists
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, dirname) == 0 && files[i].parent_dir == current_dir) {
            print("Error: Directory already exists\n");
            return;
        }
    }
    
    // Find an empty slot
    int dir_index = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] == '\0') {
            dir_index = i;
            break;
        }
    }
    
    if (dir_index == -1) {
        print("Error: No space left in file system\n");
        return;
    }
    
    // Create directory entry
    strcpy(files[dir_index].name, dirname);
    files[dir_index].start = 0;  // Directories don't have data
    files[dir_index].size = 0;
    files[dir_index].type = DIR_TYPE;
    files[dir_index].parent_dir = current_dir;
    
    print("Directory created successfully\n");
}

void command_rm(const char* filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, filename) == 0 && files[i].parent_dir == current_dir) {
            if (files[i].type == DIR_TYPE) {
                // Check if directory is empty
                int is_empty = 1;
                for (int j = 0; j < MAX_FILES; j++) {
                    if (files[j].parent_dir == i) {
                        is_empty = 0;
                        break;
                    }
                }
                
                if (!is_empty) {
                    print("Error: Directory not empty\n");
                    return;
                }
            }
            
            // Clear the file entry
            files[i].name[0] = '\0';
            files[i].start = 0;
            files[i].size = 0;
            files[i].type = 0;
            files[i].parent_dir = 0;
            
            print("Deleted successfully\n");
            return;
        }
    }
    
    print("File or directory not found\n");
}

void command_cd(const char* dirname) {
    if (strcmp(dirname, "..") == 0) {
        // Go to parent directory
        if (current_dir != 0) {  // Not already at root
            current_dir = files[current_dir].parent_dir;
            
            // Update current path
            char* last_slash = strrchr(current_path, '/');
            if (last_slash != current_path) {
                *last_slash = '\0';  // Remove last directory from path
            } else {
                current_path[1] = '\0';  // Keep just the root slash
            }
        }
        return;
    }
    
    if (strcmp(dirname, "/") == 0) {
        // Go to root directory
        current_dir = 0;
        strcpy(current_path, "/");
        return;
    }
    
    // Find the directory
    for (int i = 0; i < MAX_FILES; i++) {
        if (strcmp(files[i].name, dirname) == 0 && files[i].parent_dir == current_dir) {
            if (files[i].type != DIR_TYPE) {
                print("Error: Not a directory\n");
                return;
            }
            
            current_dir = i;
            
            // Update current path
            if (current_path[strlen(current_path) - 1] != '/') {
                strcat(current_path, "/");
            }
            strcat(current_path, dirname);
            
            return;
        }
    }
    
    print("Directory not found\n");
}

void command_pwd() {
    print(current_path);
    print("\n");
}

// Helper function to find the last occurrence of a character in a string
char* strrchr(const char* str, char ch) {
    char* last = NULL;
    while (*str) {
        if (*str == ch) {
            last = (char*)str;
        }
        str++;
    }
    return last;
}
