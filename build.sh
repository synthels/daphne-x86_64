#!/bin/sh

# phiOS Build script

# Point this to your nasm binary
NASM="nasm"
# Point this to your gcc binary
GCC="i686-elf-gcc"
# Point this to your qemu binary
QEMU="qemu-system-i386"

# Point this to your as binary
AS="i686-elf-as"

# Point this to your build directory
# Should already contain boot/grub/grub.cfg
BUILD_DIR="build"

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
    mkdir $BUILD_DIR
    mkdir $BUILD_DIR/boot
    mkdir $BUILD_DIR/boot/grub
    touch $BUILD_DIR/boot/grub/grub.cfg
fi

# Overwrite grub.cfg
echo "default=0
timeout=15

menuentry \"phiOS\" {
    multiboot /boot/kernel.bin
}" > $BUILD_DIR/boot/grub/grub.cfg

# Assemble bootloader
$AS kernel/boot/boot.s -o $BUILD_DIR/boot.o
# Assemble IDT
$NASM -f elf32 kernel/idt/irq.s -o $BUILD_DIR/irq.o
# Assemble GDT
$NASM -f elf32 kernel/boot/gdt.s -o $BUILD_DIR/gdt.o

# Compile rest of kernel
for file in $(find kernel/kmain -name '*.c'); do
    echo "Compiling kernel source file: $file..."
    $GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel
done

for file in $(find kernel/io -name '*.c'); do
    echo "Compiling kernel source file: $file..."
    $GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel
done

for file in $(find kernel/idt -name '*.c'); do
    echo "Compiling kernel source file: $file..."
    $GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel
done

for file in $(find kernel/drivers -name '*.c'); do
    echo "Compiling kernel source file: $file..."
    $GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel
done

for file in $(find kernel/tty -name '*.c'); do
    echo "Compiling kernel source file: $file..."
    $GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel
done

echo "Linking..."

# Link everything together
$GCC -g -T linker.ld -o $BUILD_DIR/boot/kernel.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/*.o -lgcc

# Clean build directory
rm $BUILD_DIR/*.o

# Build ISO & exit if requested
if ! [ $# -eq 0 ]
then
    if [ "$1" = "iso" ]
    then
        echo "Building ISO..."
        grub-mkrescue -o phiOS.iso $BUILD_DIR
    fi
fi

read -p "Press ENTER to run qemu"
# Run qemu
$QEMU -kernel $BUILD_DIR/boot/kernel.bin -soundhw pcspk
