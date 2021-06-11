#!/bin/sh

mkdir cross
cd cross

export PREFIX="$(pwd)"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export GCC_VER="11.1.0"
export BINUTILS_VER="2.36"

cd ~

# build binutils
echo "building binutils..."
wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.gz
tar -xvf binutils-$BINUTILS_VER.tar.gz
mkdir build-binutils
cd build-binutils
../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls
make
make install
cd ..

# build gcc
echo "building gcc..."
# install multilib
sudo apt-get install gcc-multilib
# install gcc
wget http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.gz
tar xzf gcc-$GCC_VER.tar.gz
cd gcc-$GCC_VER
./contrib/download_prerequisites
cd ..
mkdir objdir
cd objdir
# build gcc
$PWD/../gcc-$GCC_VER/configure --target=$TARGET --prefix=$HOME/GCC-$GCC_VER --enable-languages=c
make
make install
