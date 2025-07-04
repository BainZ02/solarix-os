#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

uint16_t* const VGA_MEMORY = (uint16_t*) VGA_ADDRESS;
int terminal_row = 0;
int terminal_col = 0;
uint8_t terminal_color = 0x0F;

void terminal_clear() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}

void terminal_putchar(char c) {
    if (c == '\\n') {
        terminal_row++;
        terminal_col = 0;
        return;
    }
    const size_t index = terminal_row * VGA_WIDTH + terminal_col;
    VGA_MEMORY[index] = ((uint16_t)terminal_color << 8) | c;
    terminal_col++;
    if (terminal_col == VGA_WIDTH) {
        terminal_col = 0;
        terminal_row++;
    }
}

void terminal_write(const char* str) {
    while (*str) terminal_putchar(*str++);
}

void terminal_writeln(const char* str) {
    terminal_write(str);
    terminal_putchar('\\n');
}

bool authenticated = false;

void login_screen() {
    terminal_writeln("Solarix OS Login");
    terminal_writeln("-----------------");
    terminal_write("Username: root\\n");
    terminal_write("Password: ********\\n");
    terminal_writeln("\\nLogin successful!");
    authenticated = true;
}

void shell_prompt() {
    terminal_write("root@solarix:~# ");
}

void handle_command(const char* cmd) {
    if (!cmd) return;
    if (strcmp(cmd, "help") == 0) {
        terminal_writeln("Commands: help, clear, about");
    } else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(cmd, "about") == 0) {
        terminal_writeln("Solarix OS v1.0 - Minimal x86 OS");
    } else {
        terminal_writeln("Unknown command");
    }
}

void kernel_main() {
    terminal_clear();
    login_screen();
    if (authenticated) {
        shell_prompt();
        handle_command("about");
        shell_prompt();
        handle_command("help");
        shell_prompt();
    }
}
