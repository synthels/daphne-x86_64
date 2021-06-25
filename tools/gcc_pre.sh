#!/bin/sh

####################################
echo Stage 1 - Building Dependencies
####################################

# make a working directory
cd $HOME/Documents
rm -rf Cross
mkdir Cross
cd Cross

# install or update all apt-get dependencies
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install gcc -y
sudo apt-get install g++ -y
sudo apt-get install make -y
sudo apt-get install bison -y
sudo apt-get install flex -y
sudo apt-get install gawk -y
sudo apt-get install libgmp3-dev -y
sudo apt-get install libmpfr-dev libmpfr-doc libmpfr4 libmpfr4-dbg -y
sudo apt-get install mpc -y
sudo apt-get install texinfo -y            # optional
sudo apt-get install libcloog-isl-dev -y   # optional
sudo apt-get install build-essential -y
sudo apt-get install glibc-devel -y
sudo apt-get -y install gcc-multilib libc6-i386 -y

# download and unpack necessary files
wget http://ftpmirror.gnu.org/binutils/binutils-2.25.1.tar.gz
wget http://ftpmirror.gnu.org/mpc/mpc-1.0.3.tar.gz
for f in *.tar*; do tar zvxf $f; done

# create installation directory
mkdir Install
export PREFIX="$HOME/Documents/Cross/Install"

################################
echo Stage 2 - Building Compiler
################################

# install mpc
mkdir build-mpc
cd build-mpc
../mpc-1.0.3/configure --prefix="$PREFIX"
make -j2
make -j2 check
make -j2 install
cd ..