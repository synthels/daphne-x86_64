#!/bin/sh

# Kernel build script

# Point this to your nasm binary
NASM="nasm"
# Point this to your gcc binary
GCC="i686-elf-gcc"
# Point this to your qemu binary
QEMU="qemu-system-i386"

# Point this to your as binary
AS="i686-elf-as"

# Build directory name
BUILD_DIR="build"

# Overwrite grub.cfg
echo "default=0
timeout=15

menuentry \"phiOS\" {
	multiboot /boot/kernel.bin
}" > $BUILD_DIR/boot/grub/grub.cfg

# Assemble bootloader
$NASM -f elf32 kernel/boot/boot.s -o $BUILD_DIR/boot.o
# Assemble IDT
$NASM -f elf32 kernel/idt/irq.s -o $BUILD_DIR/irq.o
# Assemble GDT
$NASM -f elf32 kernel/boot/gdt.s -o $BUILD_DIR/gdt.o
$NASM -f elf32 kernel/boot/paging.s -o $BUILD_DIR/paging.o

function compileModule() {
	for file in $(find kernel/$1 -name '*.c'); do
		echo "Compiling kernel source file: $file..."
		$GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel -Wno-maybe-uninitialized
	done
}

compileModule "kmain"
compileModule "io"
compileModule "idt"
compileModule "modules"
compileModule "tty"
compileModule "memory"
compileModule "logger"

echo "Linking..."

# Link everything together
$GCC -g -T linker.ld -o $BUILD_DIR/boot/kernel.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/*.o -lgcc

# Clean build directory
rm $BUILD_DIR/*.o

# Parse arguments
for arg in "$@"; do
	if [ $arg == "-iso" ]; then
		echo "Building ISO..."
		grub-mkrescue -o phiOS.iso $BUILD_DIR
	elif [ $arg == "-qemu" ]; then
		read -p "Press ENTER to run qemu"
		$QEMU -kernel $BUILD_DIR/boot/kernel.bin -soundhw pcspk
	fi
done
