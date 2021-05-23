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
# Set this to your target platform
ARCH_TARGET="x86_32"

# Overwrite grub.cfg
echo "default=0
timeout=15

menuentry \"phiOS\" {
	multiboot /boot/kernel.bin
	boot
}" > $BUILD_DIR/boot/grub/grub.cfg

# Assemble bootsector
$AS kernel/arch/$ARCH_TARGET/boot/boot.s -o $BUILD_DIR/boot.o
# Assemble IDT
$NASM -f elf32 kernel/idt/irq.s -o $BUILD_DIR/irq.o
# Assemble GDT
$NASM -f elf32 kernel/arch/$ARCH_TARGET/boot/gdt.s -o $BUILD_DIR/gdt.o
# Assemble paging code
$NASM -f elf32 kernel/arch/$ARCH_TARGET/boot/paging.s -o $BUILD_DIR/paging.o

function compileModule() {
	for file in $(find kernel/$1 -name '*.c'); do
		echo "Compiling kernel source file: $file..."
		$GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel -Wno-maybe-uninitialized
	done
}

compileModule "arch/$ARCH_TARGET"
compileModule "kmain"
compileModule "io"
compileModule "idt"
compileModule "drivers"
compileModule "tty"
compileModule "memory"
compileModule "logger"

echo "Linking..."

# Link everything together
$GCC -g -T kernel/linker.ld -o $BUILD_DIR/boot/kernel.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/*.o -lgcc

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
