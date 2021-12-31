# daphne

Daphne aims to be a modern and elegant 64-bit operating currently targetted at the x86 family of processors.

## Acknowledgments

- [Limine](https://github.com/limine-bootloader/limine) - An x86/x64 bootloader

## License

daphne is licensed under the GPLv3 license. See [license](https://github.com/synthels/daphne/blob/master/license) for more details.

## Build guide

### Supported environments

- Any GNU/Linux distro
- WSL
- MinGW (will probably work, but not very well tested)

### Supported emulators

- QEMU
- Virtualbox
- VMWare

We use CMake as our build system, so you will have to install that first of all.

If you haven't built gcc already, please run the `tools/gcc_pre.sh` script first.
Before building, you must also build the `x86_64-elf-gcc` compiler. The easiest way to build it is to use brew like so

```bash
$ brew install x86_64-elf-gcc
```

#### WSL note

On WSL/Some linux distros, you may have to run these 2 commands every time you start up the shell, otherwise CMake will not be able to find the compiler

```bash
$ export BREW_HOME="/home/linuxbrew/.linuxbrew/bin"
```

```bash
$ export PATH="$PATH:$BREW_HOME"
```

If there are no errors and everything went well, you should now have a multiboot compliant binary called `kernel.bin` in the `build` directory. Congratulations! (If by any chance it didn't go quite that well and instead gcc gave you a bunch of errors, then fix them! It's not my fault you can't write C!)

### Building for x64

First, clone the repositorty with the following command

```bash
$ git clone https://github.com/synthels/daphne.git --recursive
```

#### Building limine

building limine is really simple. All you have to do is run `make` from inside the `kernel/arch/x86_64/limine` directory.

#### Building the kernel

First, run

```bash
$ ./tools/setup.sh -x64
```

from the root directory in order to create the `build` directory. In this directory you will find the `iso` directory and a clean script. Don't worry about these for now.

Then run

```bash
$ cd build
```

Now that you're in the `build` directory, we can start fiddling with CMake!

```bash
$ cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=x86_64-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_64 ..
```

This command will create all the necessary files for CMake to build the project. If by this stage you wish to build the tests along with the kernel, add the `-DBUILD_TESTS=all` option to the previous command.

To finally build the binary, you can run CMake's generic build command

```bash
$ cmake --build .
```

If there are no errors and everything went well, you should now have a binary called `kernel.bin` in the `build` directory. Congratulations! (If by any chance it didn't go quite that well and instead gcc gave you a bunch of errors, then fix them! It's not my fault you can't write C!)

### Building an ISO image

#### x86_64 UEFI/BIOS

In order to build an ISO image under x86_64, follow these instructions.
First, `cd` to `kernel/arch/x86_64/limine/bin`. Then, run the following commands

```bash
$ cp -v limine.sys limine-cd.bin limine-eltorito-efi.bin ../../../../../build/iso/
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

if you also want BIOS, you might want to run these commands

```bash
$ cd ../kernel/arch/x86_64/limine/bin
$ ./limine-install ../../../../../build/daphne_img_x64.iso
```

#### Testing x64 images in Virtualbox

First, make sure the "Enable EFI (special OSes only)" checkbox under system is checked. Then, once
you point the VM to the ISO image, start the VM. When in the UEFI shell, type this sequence of commands

```bash
$ fs1:
$ cd EFI/BOOT
$ BOOTX64.EFI
```

in an x86-64 virtual machine such as VirtualBox or QEMU. It contains a complete

#### Testing x64 images in QEMU

Testing under QEMU requires that you have an OVMF image installed, then, at least under linux, you can use
this command

```bash
# UEFI
$ qemu-system-x86_64 --bios /usr/share/qemu/OVMF.fd -m 2048 -cdrom daphne_img_x64.iso
# Legacy BIOS
$ qemu-system-x86_64 -m 2048 -cdrom daphne_img_x64.iso
```

## Contributing to daphne

All contributions to daphne are very welcome and needed. If you have decided to contribute to this project, all you have to do is follow a few minor guidelines.

### Table of contents

- [Writing commit messages](#commit-messages)
- [Code style](#code-style)
  - [Functions](#functions)
  - [Comments](#comments)
  - [If statements and loops](#if-statements-and-loops)
  - [Header files](#header-files)

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

I ask that you follow our code style, since it makes the codebase more pretty and organized. First of all, we only use spaces. Please indent your code with 4 spaces when writing C and CMakeLists.txt files.

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
 * Copyright (C) 2020 (name) (email)
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
 * Copyright (C) 2020 (name) (email)
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
 * Copyright (C) 2020 (name) (email)
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
