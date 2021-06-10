SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
BUILD_DIR="$SCRIPT_DIR/../build"

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
	mkdir $BUILD_DIR
fi

if [[ $1 = "-mk-grub" ]]
then
	mkdir $BUILD_DIR/iso
	mkdir $BUILD_DIR/iso/boot
	mkdir $BUILD_DIR/iso/boot/grub
	touch $BUILD_DIR/iso/boot/grub/grub.cfg
fi

# Overwrite grub.cfg
echo "default=0
timeout=0

menuentry \"TauOS\" {
	echo -n \"Loading kernel...\"
	multiboot /boot/kernel.bin
	boot
}" > $BUILD_DIR/iso/boot/grub/grub.cfg

# Add a clean script to build
touch $BUILD_DIR/clean.sh

echo "rm -r ./*
./../scripts/setup.sh -mk-grub" > $BUILD_DIR/clean.sh
