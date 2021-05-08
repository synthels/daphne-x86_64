#!/bin/bash

# Thanks to https://github.com/mental32 for this script!

VERSION=0.1.0

BINUTILS_VERSION=2.32
GCC_VERSION=8.3.0
GDB_VERSION=8.2.1

BUILD_DIR="$HOME/.i686-elf"
export PATH="$BUILD_DIR/bin:$PATH"

function extract {
	name=$1
	version=$2
	tar -xf $name-$version.tar.gz
	rm -f $name-$version.tar.gz
}

function fresh {
	cd $BUILD_DIR
	clear
}

function main {
	mkdir -pv $BUILD_DIR
	cd $BUILD_DIR

	mkdir -v bin
	mkdir -v .bin
	mkdir -v build

	download
	compile
	sudo find bin -type f -perm /a+x -exec mv -f {} $BUILD_DIR/.bin \;
	sudo rm -rf bin
	sudo rm -rf build
	sudo mv .bin bin
}

function download {
	echo " => Downloading binutils"
	wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
	echo " => Extracting binutils"
	extract "binutils" $BINUTILS_VERSION

	echo " => Downloading GCC"
	wget http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz
	echo " => Extracting GCC"
	extract "gcc" $GCC_VERSION

	echo " => Downloading GDB"
	wget http://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.gz
	echo " => Extracting GDB"
	extract "gdb" $GDB_VERSION
}

function compile {
	echo " => Compiling GDB"
	mkdir build/gdb
	cd build/gdb
	../../gdb-$GDB_VERSION/configure --target=i686-elf --disable-nls --disable-werror --prefix=$BUILD_DIR/bin
	sudo make install
	fresh
	sudo rm -rf gdb-$GDB_VERSION

	echo " => Compiling binutils"
	mkdir -p build/binutils
	cd build/binutils
	../../binutils-$BINUTILS_VERSION/configure --target=i686-elf --with-sysroot --disable-nls --disable-werror --prefix=$BUILD_DIR/bin
	make
	sudo make install
	fresh
	sudo rm -rf binutils-$BINUTILS_VERSION

	echo " => Compiling GCC"
	mkdir build/gcc
	cd build/gcc
	../../gcc-$GCC_VERSION/configure --target=i686-elf --disable-nls --enable-languages=c,c++ --without-headers --prefix=$BUILD_DIR/bin
	make all-gcc
	sudo make install-gcc
	make all-target-libgcc
	sudo make install-target-libgcc
	fresh
	sudo rm -rf gcc-$GCC_VERSION    
}

read -p "Please stand by while the installation proceeds, you will be asked for your password at some point. (Press ENTER)"
main
