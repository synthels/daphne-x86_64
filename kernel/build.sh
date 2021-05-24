#!/bin/sh

# Kernel build script

# Assemble bootsector
$AS kernel/arch/$ARCH_TARGET/boot/boot.s -o $BUILD_DIR/boot.o
# Assemble IDT
$NASM -f elf32 kernel/idt/irq.s -o $BUILD_DIR/irq.o
# Assemble GDT
$NASM -f elf32 kernel/arch/$ARCH_TARGET/boot/gdt.s -o $BUILD_DIR/gdt.o
# Assemble paging code
$NASM -f elf32 kernel/arch/$ARCH_TARGET/boot/paging.s -o $BUILD_DIR/paging.o

KernelSubModule "arch/$ARCH_TARGET"
KernelSubModule "kmain"
KernelSubModule "io"
KernelSubModule "idt"
KernelSubModule "drivers"
KernelSubModule "tty"
KernelSubModule "memory"
KernelSubModule "logger"

# Link everything together
$GCC -g -T kernel/linker.ld -o $BUILD_DIR/boot/kernel.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/*.o -lgcc
