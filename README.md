# daphne

<img src=".github/logo.png" alt="logo" style="height: 200px; width:200px;"/>

Daphne is a UNIX-like 64-bit operating system written in pure C.

## Contents

*   [Build guide](#build-guide)
    *   [Supported environments](#supported-environments)
    *   [Tested emulators](#tested-emulators)
    *   [Building gcc](#building-gcc)
    *   [Installing NASM](#installing-nasm)
    *   [Brew note](#brew-note)
    *   [Building for x86_64](#building-for-x86_64)
    *   [Building limine](#building-limine)
    *   [Building the kernel](#building-the-kernel)
    *   [Building an ISO image](#building-an-iso-image)
    *   [x86_64 UEFI/BIOS](#x86_64-uefi/bios)
    *   [Testing x64 images in Virtualbox](testing-x64-images-in-virtualbox)
    *   [Testing x64 images in QEMU](#testing-x64-images-in-qemu)

*   [Contributing to daphne](#contributing-to-daphne)
    *   [Functions](#functions)
    *   [Comments](#comments)
    *   [If statements and loops](#if-statements-and-loops)
    *   [Header files](#header-files)

## Build guide

### Supported environments

- Any GNU/Linux distro
- WSL
- MinGW (will probably work, but not very well tested)

### Tested emulators

- QEMU
- Virtualbox
- VMWare

We use CMake as our build system, so you will have to install that first of all (see https://cmake.org/install/).

### Building gcc

You can build the toolchain yourself using the scripts under the `tools` directory. Alternatively you can use brew like so:

```bash
$ brew install x86_64-elf-gcc
```

### Installing NASM

In order to build the kernel, you will also need nasm installed, which is our assembler of choice. You can install it as so under different linux distributions:

```bash
$ sudo apt-get install nasm # Ubuntu
$ sudo dnf install nasm # Fedora
$ pacman -S nasm # Arch
```

#### Brew note

If you installed gcc using brew, you may have to run these two commands before building the kernel, or CMake will not be able to find the compiler.

```bash
$ export BREW_HOME="/home/linuxbrew/.linuxbrew/bin"
$ export PATH="$PATH:$BREW_HOME"
```

### Building for x86_64

First, clone the repositorty with the following command

```bash
$ git clone https://github.com/synthels/daphne.git --recursive
```

#### Building limine

Building limine is really simple. All you have to do is run `./autogen.sh` and then `make` from inside the `src/kernel/arch/x86_64/limine` directory.

#### Building the kernel

Change your working directory to the root directory (`daphne/`) & follow this guide.

First, run

```bash
$ ./tools/setup.sh -x64
```

which will setup the build environment for x86_64.

Then run

```bash
$ cd build
$ cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=x86_64-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_64 ..
```

This command will create all the necessary files for CMake to build the project. If by this stage you wish to build the tests along with the kernel, add the `-DBUILD_TESTS=all` option to the previous command.

To finally build the binary, you can run CMake's generic build command.

```bash
$ cmake --build .
```

If there are no errors and everything went well, you should now have a binary called `kernel.bin` in the `build` directory. Congratulations! (If by any chance it didn't go quite that well and instead gcc gave you a bunch of errors, then go back and fix them! It's not my fault you can't write C!).

### Building an ISO image

#### x86_64 UEFI/BIOS

In order to build an ISO image under x86_64, follow these instructions.
First, `cd` to `src/kernel/arch/x86_64/limine/bin`. Then, run the following commands.

```bash
$ cp -v limine.sys limine-cd.bin limine-eltorito-efi.bin ../../../../../../build/iso/
```

Now, `cd` to `build` (not `limine/build`)

```bash
$ cp -v kernel.bin iso
$ xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso -o daphne_img_x64.iso
```

if you also want to boot on BIOS, you might need to install limine

```bash
$ cd ../src/kernel/arch/x86_64/limine/bin
$ ./limine-install ../../../../../../build/daphne_img_x64.iso
```

#### Testing x64 images in Virtualbox

First, make sure the "Enable EFI (special OSes only)" checkbox under system is checked. Then, once
you point the VM to the ISO image, start the VM. When in the UEFI shell, type this sequence of commands.

```bash
$ fs1:
$ cd EFI/BOOT
$ BOOTX64.EFI
```

#### Testing x64 images in QEMU

Testing under QEMU with UEFI requires that you have an OVMF image installed. If not, you can always just use leagacy BIOS. These commands will do on most linux systems:

```bash
# UEFI
$ qemu-system-x86_64 --bios /usr/share/qemu/OVMF.fd -smp cores=6 -m 2048 -cdrom daphne_img_x64.iso
# Legacy BIOS
$ qemu-system-x86_64 -m 2048 -cdrom daphne_img_x64.iso -smp cores=6
```

## Contributing to daphne

All contributions to daphne are very welcome and needed. If you have decided to contribute to this project, all you have to do is follow a few minor guidelines.

### Commit messages

When writing a commit message, please make sure it is in the format of

```
category: what you did here...
```

For example, if I fixed a bug in the `printk` function under `tty`, my commit message would be something like this

```
tty: printk bugfixes
```

You can go into more detail, but make sure your commit messages aren't too long. While you are at it, make sure that the commits themselves aren't long. Split up large commits into smaller ones so that we don't end up with another horror like this [one](https://github.com/synthels/daphne/commit/51416efe92011e22a2f18008b4edc683bf8d8d42)...

#### Code style

I ask that you follow our code style, since it makes the codebase more pretty and organized. First of all, we only use spaces. Please indent your code with 4 spaces when writing C and CMakeLists.txt files. I also ask that you leave an empty line at the end of every file. This can usually be configured to be done automagically by your text editor.

##### Functions

All of the function singature must be on the same line, only putting the opening brace on the next.

```c
void foo(int p1, int p2)
{
    /* ... */
}
```

##### Comments

Please only write comments like this:

```c
/* Good comment! */
```

Not like this:

```c
// Bad comment :(
```

##### If statements and loops

All if statements, for and while loops must be written with a space left between the for/while or if and the opening parenthesis. The opening brace must be put on the same line.

```c
for (int i = 0; i < size; i++) {
    /* ... */
}
```

##### Header files

Header files must begin with a copyright header, like this

```c
/*
 * Copyright (C) 2022 (name) (email)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
```

You also need the same header on source files, like this:

```c
/*
 * Copyright (C) 2022 (name) (email)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * Short description of code
 */
```

All header files must follow C convention, leaving us with a format like this:

```c
/*
 * Copyright (C) 2022 (name) (email)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#pragma once

/* All includes go here... */
```
