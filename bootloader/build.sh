#!/bin/bash
mkdir -p bin build
nasm -f bin src/boot.asm -o bin/boot.bin
nasm -f elf32 src/kernel.asm -o build/kernel.o
/home/ayush/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -O0 -I./inc -c src/kernel.c -o build/kernel_c.o
/home/ayush/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -O0 -I./inc -c src/screen.c -o build/screen.o
/home/ayush/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -O0 -I./inc -c src/keyboard.c -o build/keyboard.o
/home/ayush/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -O0 -I./inc -c src/string.c -o build/string.o
/home/ayush/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -O0 -I./inc -c src/fs.c -o build/fs.o
/home/ayush/opt/cross/bin/i686-elf-gcc -T ./src/linkerScript.ld build/kernel.o build/kernel_c.o build/screen.o build/keyboard.o build/string.o build/fs.o -o build/kernel_full.o -ffreestanding -nostdlib -lgcc
objcopy -O binary build/kernel_full.o bin/kernel.bin
cat bin/boot.bin bin/kernel.bin > bin/os.bin
truncate -s 1474560 bin/os.bin
ls -la bin/os.bin
