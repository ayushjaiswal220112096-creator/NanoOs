ASM = nasm
CC = i686-elf-gcc
CFLAGS = -ffreestanding -nostdlib -O0 -I./inc
LDS = ./src/linkerScript.ld

.PHONY: all clean run

all:
	mkdir -p bin build
	$(ASM) -f bin src/boot.asm -o bin/boot.bin
	$(ASM) -f elf32 src/kernel.asm -o build/kernel.o
	$(CC) $(CFLAGS) -c src/kernel.c -o build/kernel_c.o
	$(CC) $(CFLAGS) -c src/screen.c -o build/screen.o
	$(CC) $(CFLAGS) -c src/keyboard.c -o build/keyboard.o
	$(CC) $(CFLAGS) -c src/string.c -o build/string.o
	$(CC) $(CFLAGS) -c src/fs.c -o build/fs.o
	$(CC) -T $(LDS) build/kernel.o build/kernel_c.o build/screen.o build/keyboard.o build/string.o build/fs.o -o build/kernel_full.o $(CFLAGS) -lgcc
	objcopy -O binary build/kernel_full.o bin/kernel.bin
	cat bin/boot.bin bin/kernel.bin > bin/os.bin
	truncate -s 1474560 bin/os.bin
	ls -la bin/os.bin

run:
	qemu-system-i386 -drive format=raw,file=bin/os.bin -serial file:serial.log -monitor stdio

clean:
	rm -f bin/*.bin build/*.o
