BUILD_DIR="build"

# Create build directory if it doesn't exist
if ! [[ -d "$BUILD_DIR" ]]
then
	mkdir $BUILD_DIR
	mkdir $BUILD_DIR/iso
	mkdir $BUILD_DIR/iso/boot
	mkdir $BUILD_DIR/iso/boot/grub
	touch $BUILD_DIR/iso/boot/grub/grub.cfg
fi

# Overwrite grub.cfg
echo "default=0
timeout=15

menuentry \"phiOS\" {
	multiboot /boot/kernel.bin
	boot
}" > $BUILD_DIR/iso/boot/grub/grub.cfg
