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
        print("  help        - Display this help message\n");
        print("  cleanscreen - Clear the screen\n");
        print("  list        - List files and directories\n");
        print("  view        - Display file contents (usage: view filename)\n");
        print("  write       - Create or overwrite a file (usage: write filename content)\n");
        print("  newdir      - Create a directory (usage: newdir dirname)\n");
        print("  del         - Remove a file or empty directory (usage: del name)\n");
        print("  goto        - Change directory (usage: goto dirname)\n");
        print("  curdir      - Print working directory\n");
        print("  about       - About NanoOS\n");
    }
    else if (strcmp(cmd, "cleanscreen") == 0) {
        clear_screen();
    }
    else if (strcmp(cmd, "list") == 0) {
        command_ls();
    }
    else if (strncmp(cmd, "view ", 5) == 0) {
        command_cat(cmd + 5); // Skip "view " prefix
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
    else if (strncmp(cmd, "newdir ", 7) == 0) {
        command_mkdir(cmd + 7);
    }
    else if (strncmp(cmd, "del ", 4) == 0) {
        command_rm(cmd + 4);
    }
    else if (strncmp(cmd, "goto ", 5) == 0) {
        command_cd(cmd + 5);
    }
    else if (strcmp(cmd, "curdir") == 0) {
        command_pwd();
    }
    else if (strcmp(cmd, "about") == 0) {
        print("NanoOs - A simple shell-based operating system built for learning purposes.\n\n");
        print("Developed by Ayush Jaiswal, Arpit Upadhyay, Sonakshi Chand, and Garima Tiwari\n");
        print("as part of a Project-Based Learning (PBL) initiative for B.Tech VI Semester\n");
        print("at Graphic Era Hill University.\n\n");
        print("Currently features basic shell commands and a simple file system.\n\n");
        print("More features will be added in future updates!\n");
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
