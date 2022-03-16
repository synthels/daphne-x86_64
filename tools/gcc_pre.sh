#!/bin/bash

####################################
echo Stage 1 - Building Dependencies
####################################

# make a working directory
WORK_DIR="$HOME/Documents/Cross"
rm -rf "$WORK_DIR"
mkdir -p \
"$WORK_DIR/Install" \
"$WORK_DIR/build-mpc"
export PREFIX="$WORK_DIR/Install"
cd "$WORK_DIR"

# install and update all apt-get dependencies
sudo apt-get update && sudo apt-get upgrade -y
sudo apt-get install -y \
gcc g++ make bison \
flex gawk mpc build-essential \
libgmp3-dev libmpfr-dev libmpfr-doc libmpfr4 \
libmpfr4-dbg glibc-devel gcc-multilib libc6-i386
# optional dependencies
sudo apt-get install -y \
texinfo libcloog-isl-dev

# download and unpack necessary files
wget http://ftpmirror.gnu.org/binutils/binutils-2.25.1.tar.gz
wget http://ftpmirror.gnu.org/mpc/mpc-1.0.3.tar.gz
for f in *.tar*; do tar zvxf $f; done

################################
echo Stage 2 - Building Compiler
################################

# install mpc
cd build-mpc
../mpc-1.0.3/configure --prefix="$PREFIX"
make -j2
make -j2 check
make -j2 install
cd "$WORK_DIR"
