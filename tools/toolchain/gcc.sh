#!/bin/bash

# DO NOT CALL THIS SCRIPT DIRECTLY!
# Expects the first argument to be the build directory, the second to
# be the source directory, the third to be the number of cores it should
# use while compiling, the fourth to be the prefix and the fifth to be the sysroot

cd $1

$2/configure --prefix="$4" --target=x86_64-forbia --with-sysroot="$5" --enable-languages=c,c++ --disable-multilib --disable-nls CFLAGS=-O2 CXXFLAGS=-O2

make -j"$3" all-gcc
make install-gcc

make -j"$3" all-target-libgcc
make install install-target-libgcc
