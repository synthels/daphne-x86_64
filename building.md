# Build guide

## Supported environments
* Any GNU/Linux distro
* WSL
* MinGW (will probably work, but not very well tested)

## Supported emulators
* QEMU
* Virtualbox
* VMWare

We use CMake as our build system, so you will have to install that first of all.

If you haven't built gcc already, please run the `tools/gcc_pre.sh` script first.
Before building, you must also build the `i686-elf-gcc` compiler. The easiest way to build it is to use brew like so

```
brew install i686-elf-gcc
```

If you're building for x64, use the following command instead

```
brew install x86_64-elf-gcc
```

### WSL note
On WSL/Some linux distros, you may have to run these 2 commands every time you start up the shell, otherwise CMake will not be able to find the compiler

```
export BREW_HOME="/home/linuxbrew/.linuxbrew/bin"
```

```
export PATH="$PATH:$BREW_HOME"
```

# Let's start the build! (x86_32 BIOS)
First, run

```
./tools/setup.sh -mk-grub
```

in order create the `build` directory. In this directory you will find the `iso` directory and a clean script. Don't worry about these for now.

Then run

```
cd build
```

Now that you're in the `build` directory, we can start fiddling with CMake!

```
cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=i686-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_32 ..
```
This command will create all the necessary files for CMake to build the project. If by this stage you wish to build the tests along with the kernel, add the `-DBUILD_TESTS=all` option to the previous command.

To finally build the binary, you can run CMake's generic build command

```
cmake --build .
```

If there are no errors and everything went well, you should now have a multiboot compliant binary called `kernel.bin` in the `build` directory. Congratulations! (If by any chance it didn't go quite that well and instead gcc gave you a bunch of errors, then fix them! It's not my fault you can't write C!)

# Let's start the build! (x86_64 UEFI)
First, in order to build the bootloader, run `make` in the `kernel/arch/x86_64/boot` directory in order to build `bootx64.c`. Now, you should refer to the instructions for `x86_32`, only replacing every `-DARCH=x86_32` with `-DARCH=x86_64` and every `-DCMAKE_C_COMPILER=i686-elf-gcc` with `-DCMAKE_C_COMPILER=x86_64-elf-gcc`. You should also run `./tools/setup.sh -mk-uefi` instead of `./tools/setup.sh -mk-grub`.

# Building an ISO image
If you're still here, you might be interested in building an ISO image. Lucky for you, I can tell you how to do just that!

First of all, run the following command to strip any debug info off the kernel binary

```
strip --strip-debug kernel.bin
```

Then, move `kernel.bin` to the `iso/boot` directory.

Next, to build the ISO, just run

```
grub-mkrescue -o daphne-img-x32.iso iso
```

## x86_64 UEFI
In order to build an ISO image under x86_64, follow these instructions.
First, `cd` to `kernel/arch/x86_64/boot`. There, run the following commands

```
dd if=/dev/zero of=fat.img bs=1k count=1440
```

```
mformat -i fat.img -f 1440 ::
```

to format our FAT image. Next, run

```
mmd -i fat.img ::/EFI
```

```
mmd -i fat.img ::/EFI/BOOT
```

```
mcopy -i fat.img bootx64.efi ::/EFI/BOOT
```

Now, move `fat.img` to `build/iso` and run

```
xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o daphne_img_x64.iso iso
```

from the `build` directory

** TODO: Instructions incomplete, need to show how to include kernel **
