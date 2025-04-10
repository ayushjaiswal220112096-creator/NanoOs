# NanoOS

**NanoOS** is a lightweight, custom-built operating system designed with a **monolithic kernel architecture**.  
This project is created as a learning initiative to explore the fundamentals of OS development, including:

- Bootloading  
- Kernel design  
- Memory management  
- File systems  
- Basic user interface

---

## 🛠️ Features (Implemented / Planned)

- ✅ Custom bootloader (BIOS-based)
- ✅ Monolithic kernel in C
- ✅ Basic CLI-based shell
- ✅ Memory management (paging and segmentation)
- ✅ Keyboard input handling
- ✅ Screen output with custom drivers
- 🔲 File system integration (FAT12/FAT32)
- 🔲 Process and task management
- 🔲 System call interface
- 🔲 Multitasking support
- 🔲 Basic user-space programs

---

## 🧩 Architecture

### Monolithic Kernel Design

- All OS services run in the same address space (**kernel space**)
- No user-kernel mode switch for service calls, resulting in better performance
- Modules are tightly integrated and compiled together into a single binary

---

## 🤖 Tools & Technologies
- Languages: C, Assembly

- Emulator: QEMU

- Assembler: NASM

- Cross-Compiler: GCC (i386 ELF)

- Platform: WSL / Linux
---
## 👨‍💻 Contributors
- Sonakshi / Garima – Kernel & Driver Development
- Ayush / Arpit – Bootloader & System Initialization
