#include "kernel.h"
#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "fs.h"
#include <stddef.h>

#define CMD_BUFFER_SIZE 256

void execute_command(char* cmd);

// Kernel entry point
void kernel_main() {
    char cmd_buffer[CMD_BUFFER_SIZE];
    
    // Clear the screen and display a welcome message
    clear_screen();
    print("Welcome to NanoOS!\n");
    print("A simple operating system for learning purposes.\n");
    print("-------------------------------------------------\n");
    print("\nSystem initialized successfully.\n\n");
    
    // Initialize file system
    load_directory();
    
    // Main command loop
    while(1) {
        print("NanoOS> ");
        
        // Read user input
        read_input(cmd_buffer);
        
        // Execute the command
        execute_command(cmd_buffer);
    }
}

// Process and execute commands
void execute_command(char* cmd) {
    if (strcmp(cmd, "help") == 0) {
        print("Available commands:\n");
        print("  help    - Display this help message\n");
        print("  clear   - Clear the screen\n");
        print("  ls      - List files and directories\n");
        print("  cat     - Display file contents (usage: cat filename)\n");
        print("  write   - Create or overwrite a file (usage: write filename content)\n");
        print("  mkdir   - Create a directory (usage: mkdir dirname)\n");
        print("  rm      - Remove a file or empty directory (usage: rm name)\n");
        print("  cd      - Change directory (usage: cd dirname)\n");
        print("  pwd     - Print working directory\n");
        print("  about   - About NanoOS\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    }
    else if (strcmp(cmd, "ls") == 0) {
        command_ls();
    }
    else if (strncmp(cmd, "cat ", 4) == 0) {
        command_cat(cmd + 4); // Skip "cat " prefix
    }
    else if (strncmp(cmd, "write ", 6) == 0) {
        char* filename = cmd + 6;
        char* content = strchr(filename, ' ');
        
        if (content == NULL) {
            print("Usage: write filename content\n");
            return;
        }
        
        *content = '\0'; // Split the string
        content++; // Move to the content part
        
        command_write(filename, content);
    }
    else if (strncmp(cmd, "mkdir ", 6) == 0) {
        command_mkdir(cmd + 6);
    }
    else if (strncmp(cmd, "rm ", 3) == 0) {
        command_rm(cmd + 3);
    }
    else if (strncmp(cmd, "cd ", 3) == 0) {
        command_cd(cmd + 3);
    }
    else if (strcmp(cmd, "pwd") == 0) {
        command_pwd();
    }
    else if (strcmp(cmd, "about") == 0) {
        print("NanoOS - A simple operating system for learning purposes\n");
        print("Developed as a learning project for OS development\n");
    }
    else if (strlen(cmd) == 0) {
        // Empty command, do nothing
    }
    else {
        print("Unknown command: ");
        print(cmd);
        print("\nType 'help' for available commands\n");
    }
}
