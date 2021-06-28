#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_DIR="$SCRIPT_DIR/../build"

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
	mkdir $BUILD_DIR
fi

# Add a clean script to build
touch $BUILD_DIR/clean.sh

if [[ $1 = "-mk-grub" ]]
then
	mkdir $BUILD_DIR/iso
	mkdir $BUILD_DIR/iso/boot
	mkdir $BUILD_DIR/iso/boot/grub
	touch $BUILD_DIR/iso/boot/grub/grub.cfg
	echo "default=0
	timeout=0

	menuentry \"daphne\" {
		multiboot /boot/kernel.bin
		boot
	}" > $BUILD_DIR/iso/boot/grub/grub.cfg

	echo "#!/bin/sh
	rm -r ./*
	bash ../tools/setup.sh -mk-grub" > $BUILD_DIR/clean.sh
fi

if [[ $1 = "-mk-uefi" ]]
then
	mkdir $BUILD_DIR/iso
	echo "#!/bin/sh
	rm -r ./*
	bash ../tools/setup.sh -mk-uefi" > $BUILD_DIR/clean.sh
fi
