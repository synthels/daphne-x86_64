#!/bin/sh

#########################################
# Point this to your nasm binary
NASM=nasm

# Point this to your gcc binary
GCC=i686-elf-gcc

# Point this to your qemu binary
QEMU=qemu-system-i386

# Point this to your as binary
AS=i686-elf-as

# Set this to your target platform
ARCH_TARGET=x86_32

# Build directory name
BUILD_DIR=build

# Flags
GCC_FLAGS=(-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel -Wno-maybe-uninitialized)
QEMU_FLAGS=(-soundhw pcspk)

function KernelSubModule {
	for file in $(find kernel/$1 -name '*.c'); do
		echo "Compiling kernel source file: $file..."
		$GCC -g -c $file -o $BUILD_DIR/"${file//\/}".o ${GCC_FLAGS[@]}
	done
}
#########################################

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
	boot
}" > $BUILD_DIR/boot/grub/grub.cfg

# Parse args
args=("")
for arg in "$@"; do
	if [ $arg == "-iso" ]; then
		args+=("-iso")
	elif [ $arg == "-qemu" ]; then
		args+=("-qemu")
	elif [ $arg == "-help" ]; then
		echo "Usage: phiOS.sh [options]"
		echo "Options:"
		echo " -iso   Output an ISO file alongside the binary"
		echo " -qemu  Test using QEMU"
		exit 0
	else
		echo "Unknown option \"$arg\"! (See phiOS.sh -help for assistance)"
		exit 1
	fi
done

# Build kernel
source "kernel/build.sh" ${args[@]}

# Parse arguments
for argm in "$arg"; do
	if [ $argm == "-iso" ]; then
		echo "Building ISO..."
		grub-mkrescue -o phiOS.iso $BUILD_DIR
	elif [ $argm == "-qemu" ]; then
		read -p "Press ENTER to run qemu"
		$QEMU -kernel $BUILD_DIR/boot/kernel.bin ${QEMU_FLAGS[@]}
	fi
done

# Clean build directory
rm $BUILD_DIR/*.o
