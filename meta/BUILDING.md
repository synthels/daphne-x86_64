# Building

If you would like to build daphne, you will have to follow these instructions.

## Grabbing the source code

You should clone the source code from this repository recursively, to make sure you also fetch any submodules we might have.

```sh
$ git clone https://github.com/synthels/daphne.git --recursive
```

## Building the toolchain

Building a full toolchain like this has some prerequisites which you will need to install. Some users might have them installed already, but it's good practice to check anyway. The following packages are **required**.

### Debian and Ubuntu

```sh
$ sudo apt-get install libgmp-dev libisl-dev libmpfr-dev libmpc-dev
```

### Arch and derivatives

```sh
$ sudo pacman -S gmp libisl mpfr mpc
```

Then, run the following command, which will setup your build environment for x86_64.

```sh
$ ./tools/setup.sh x86_64
```

You can now start building the toolchain. It is nothing more than a patched version of the `x86_64-elf-*` tools, which you can build and install using our very own package manager, [pkgr](https://github.com/synthels/pkgr). After following its installation instructions, you can invoke it from daphne's root directory like so:

```sh
$ python3 -m pkgr build
```

This step might take some time, so get some tea going, sit back, relax and watch as your computer goes up in flames. You will also need `nasm`, as that's our assembler of choice. You can grab it from your distribution's package manager.

```sh
$ sudo apt-get install nasm # Debian and Ubuntu
$ sudo dnf install nasm # Fedora
$ pacman -S nasm # Arch
```

## Building the kernel

Change your working directory to the build directory and run

```sh
$ cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=FULL/PATH/TO/DAPHNE/build/bin/bin/x86_64-forbia-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_64 ..
```

This command will create all the necessary files for CMake to build the project. CMake will also archive the contents of `base` into a `tar.gz` file, so that it can be loaded by the kernel at boot. For this you will only need a working version of `tar`. If by this stage you wish to build the tests along with the kernel, add the `-DBUILD_TESTS=all` option to the previous command.

To finally compile the binary, run

```sh
$ cmake --build .
```

If there are no errors and everything went well, you should now have a binary called `kernel.bin` in the build directory. Congratulations!

## Compiling an ISO image

Make sure your current directory is the build directory and run

```sh
$ cp -v kernel.bin iso
$ xorriso -as mkisofs -b limine-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot limine-eltorito-efi.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    iso -o daphne-img-x86_64.iso
```

## Testing under QEMU

Assuming you have `qemu-system-x86_64` installed, you can use the following two commands:

```sh
# UEFI
$ qemu-system-x86_64 --bios /usr/share/qemu/OVMF.fd -smp cores=6 -m 2048 -cdrom daphne-img-x86_64.iso
# Legacy BIOS
$ qemu-system-x86_64 -m 2048 -cdrom daphne-img-x86_64.iso -smp cores=6
```
