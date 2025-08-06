#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>
#include "../cpu/type.h"

void cmd_list();
void cmd_echo(char *input);
void cmd_rename(char *input);
void cmd_move(char *input);

// Simple file system simulation
typedef struct {
    char name[32];
    int size;
    int exists;
} file_entry_t;

#define MAX_FILES 10
file_entry_t files[MAX_FILES] = {
    {"boot.bin", 512, 1},
    {"kernel.bin", 4096, 1},
    {"readme.txt", 128, 1},
    {"", 0, 0}
};

void kernel_main() {
    isr_install();
    irq_install();

    asm("int $2");
    asm("int $3");

    kprint("Type something, it will go through the kernel\n"
        "Type END to halt the CPU or PAGE to request a kmalloc()\n> ");
}

void user_input(char *input) {
    char command[256];
    int i = 0;

    // Find command
    while (input[i] != ' ' && input[i] != '\0') {
        command[i] = input[i];
        i++;
    }
    command[i] = '\0';

    // Convert command to uppercase
    for (int j = 0; command[j]; j++) {
        if (command[j] >= 'a' && command[j] <= 'z') {
            command[j] = command[j] - 32;
        }
    }

    // call function
    if (strcmp(command, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(command, "LIST") == 0) {
        cmd_list();
    } else if (strcmp(command, "CLEAR") == 0) {
        clear_screen();
    } else if (strcmp(command, "ECHO") == 0) {
        cmd_echo(input);
    } else if (strcmp(command, "RENAME") == 0) {
        cmd_rename(input);
    } else if (strcmp(command, "MOVE") == 0) {
        cmd_move(input);
    } else if (strcmp(command, "HELP") == 0) {
        kprint("MyOS v1.0 - A simple operating system\n");
        kprint("Type LIST to see available commands\n");
    } else if (strcmp(command, "PAGE") == 0) {
        u32 phys_addr;
        u32 page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
    } else {
        kprint("Unknown command: ");
        kprint(input);
        kprint("\nType LIST for available commands\n");
    }
}

void cmd_list() {
    kprint("Filename\t\tSize (Bytes)\n");
    kprint("------------------------------------\n");

    int file_found = 0;
    char size_str[32]; // Used to store the converted file size string

    for (int i = 0; i < MAX_FILES; i++) {
        // Only show files where the 'exists' is 1
        if (files[i].exists) {
            kprint(files[i].name);

            if (strlen(files[i].name) < 8) {
                kprint("\t\t");
            } else {
                kprint("\t");
            }

            // Convert integer size to string
            int_to_ascii(files[i].size, size_str);
            kprint(size_str);
            kprint("\n");

            file_found = 1;
        }
    }

    if (!file_found) {
        kprint("Directory is empty.\n");
    }
}


void cmd_echo(char *input) {
    char *message = input + 5;
    while (*message == ' ') message++;

    if (*message == '\0') {
        kprint("Usage: ECHO <message>\n");
    } else {
        kprint("Echo: ");
        kprint(message);
        kprint("\n");
    }
}

void cmd_rename(char *input) {
    // format: RENAME oldname newname
    char *ptr = input + 7; // 跳过 "RENAME "
    char oldname[32], newname[32];
    int i = 0, j = 0;

    while (*ptr == ' ') ptr++;

    while (*ptr && *ptr != ' ' && i < 31) {
        oldname[i++] = *ptr++;
    }
    oldname[i] = '\0';

    while (*ptr == ' ') ptr++;

    while (*ptr && *ptr != '\0' && j < 31) {
        newname[j++] = *ptr++;
    }
    newname[j] = '\0';

    if (i == 0 || j == 0) {
        kprint("Usage: RENAME <oldname> <newname>\n");
        return;
    }

    int found = 0;
    for (i = 0; i < MAX_FILES; i++) {
        if (files[i].exists && stricmp(files[i].name, oldname) == 0) {
            strcpy(files[i].name, newname);
            kprint("File renamed: ");
            kprint(oldname);
            kprint(" -> ");
            kprint(newname);
            kprint("\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        kprint("Error: File '");
        kprint(oldname);
        kprint("' not found.\n");
    }
}

void cmd_move(char *input) {
    char *ptr = input + 5;
    char source[32], dest[32];
    int i = 0, j = 0;

    while (*ptr == ' ') ptr++;
    while (*ptr && *ptr != ' ' && i < 31) {
        source[i++] = *ptr++;
    }
    source[i] = '\0';

    while (*ptr == ' ') ptr++;
    while (*ptr && *ptr != '\0' && j < 31) {
        dest[j++] = *ptr++;
    }
    dest[j] = '\0';

    if (i == 0 || j == 0) {
        kprint("Usage: MOVE <source> <destination_path>\n");
        return;
    }

    int file_idx = -1;
    for (i = 0; i < MAX_FILES; i++) {
        if (files[i].exists && stricmp(files[i].name, source) == 0) {
            file_idx = i;
            break;
        }
    }

    if (file_idx == -1) {
        kprint("Error: File '");
        kprint(source);
        kprint("' not found.\n");
        return;
    }

    char new_full_path[32];
    strcpy(new_full_path, dest);
    strcat(new_full_path, "/");
    strcat(new_full_path, files[file_idx].name);

    if (strlen(new_full_path) >= 32) {
        kprint("Error: Destination path is too long.\n");
        return;
    }

    strcpy(files[file_idx].name, new_full_path);

    kprint("Moved '");
    kprint(source);
    kprint("' to '");
    kprint(new_full_path);
    kprint("'\n");
}