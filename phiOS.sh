#!/bin/sh

# Build directory name
BUILD_DIR="build"

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
	mkdir $BUILD_DIR
	mkdir $BUILD_DIR/boot
	mkdir $BUILD_DIR/boot/grub
	touch $BUILD_DIR/boot/grub/grub.cfg
fi

args=("")
# Parse args
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

source "kernel/build.sh" ${args[@]}
