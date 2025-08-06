run commands below to set up env
- `brew install nasm qemu gcc`
- `brew tap nativeos/i386-elf-toolchain`
- `brew install i386-elf-binutils i386-elf-gcc`

cd to /23-fixes, then you can compile and run `make run`

make sure run `make clean && make run` if you change any code.

--------------------
Purpose of the Operating System
This operating system is designed to demonstrate fundamental OS concepts and basic system functionality. The primary purpose is to:

Command Interface: Implement a simple shell interface for user interaction
Resource Management: Demonstrate basic memory management and system resource tracking
File Operations: Simulate basic file system operations like rename and move

The OS boots from a custom bootloader, initializes hardware components, and provides a command-line interface for user interaction.
Implemented Functions

1. LIST Command

Purpose: Displays all available commands in the system
Implementation: Iterates through a command table and prints each command with its description
Code Location: kernel/kernel.c - cmd_list() function

2. CLEAR Command

Purpose: Clears the terminal screen
Implementation: Writes blank spaces to the entire video memory buffer and resets cursor position
Code Location: drivers/screen.c - clear_screen() function

3. ECHO Command

Purpose: Echoes user input back to the screen
Implementation: Parses input string after "ECHO" keyword and prints it
Use Case: Testing input/output functionality
Code Location: kernel/kernel.c - cmd_echo() function

4. RENAME Command

Purpose: Demonstrates file renaming capability
Implementation: Maintains a simple file table and updates file names
Code Location: kernel/kernel.c - cmd_rename() function

5. MOVE Command

Purpose: Simulates moving files between directories
Implementation: Parses source and destination paths and simulates file movement
Code Location: kernel/kernel.c - cmd_move() function

Technical Implementation Details

Bootloader: Custom x86 bootloader that loads the kernel into memory
Kernel: Monolithic kernel design with integrated drivers
Memory Management: Basic flat memory model with simple allocation
Interrupt Handling: Implements IDT for keyboard and timer interrupts
Display Driver: Direct VGA text mode manipulation at 0xB8000

Screenshots: https://zaq7nm9t6b.feishu.cn/docx/L2rIdweXwoehnvxBxlhcZ5kKn9d?from=from_copylink

Code Statistics

Total Lines of Code: ~2,500 lines
Original Tutorial Code: ~2,000 lines
New Functions Added: ~500 lines
Languages Used: C (85%), Assembly (15%)
Files Modified: 8 files
New Functions Created: 5 major functions

Challenges and Learning Outcomes
The main challenges included understanding low-level hardware interaction, implementing proper interrupt handling, and managing memory without standard library support. This project provided valuable insights into:

How operating systems boot and initialize
Direct hardware manipulation
The importance of system calls and kernel/user space separation
Basic command parsing and execution

Future Improvements
Potential enhancements could include:

Implementing a real file system (FAT12 or custom)
Adding process management and multitasking
Implementing virtual memory with paging
Adding network stack support

Conclusion
This project successfully demonstrates core OS functionality through practical implementation. The system boots reliably, handles user input, and executes multiple commands, meeting all project requirements while providing a foundation for further OS development.