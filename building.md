# Build guide

## Supported environments
* Any GNU/Linux distro
* WSL
* MinGW (will probably work, but not very well tested)

## Supported emulators
* QEMU
* Virtualbox
* VMWare

### Bash note
The scripts in the `tools` directory seem to be throwing errors when using a shell other than bash. You
might want to check that your current shell is bash and if you don't want to fiddle with it
too much, just run all the scripts like this:

```
bash tools/script_name.sh [args]
```

We use CMake as our build system, so you will have to install that first of all.

If you haven't built gcc already, please run the `tools/gcc_pre.sh` script first.
Before building, you must also build the `x86_64-elf-gcc` compiler. The easiest way to build it is to use brew like so

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

If there are no errors and everything went well, you should now have a multiboot compliant binary called `kernel.bin` in the `build` directory. Congratulations! (If by any chance it didn't go quite that well and instead gcc gave you a bunch of errors, then fix them! It's not my fault you can't write C!)

# Let's start the build! (x86_64 UEFI)
First, in order to build the bootloader, run `make` in the `kernel/arch/x86_64/boot` directory in order to build `bootx64.c`.

Now, run

```
./tools/setup.sh -x64
```

in order create the `build` directory. In this directory you will find the `iso` directory and a clean script. Don't worry about these for now.

Then run

```
cd build
```

Now that you're in the `build` directory, we can start fiddling with CMake!

```
cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=x86_64-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_64 ..
```
This command will create all the necessary files for CMake to build the project. If by this stage you wish to build the tests along with the kernel, add the `-DBUILD_TESTS=all` option to the previous command.

To finally build the binary, you can run CMake's generic build command

```
cmake --build .
```

If there are no errors and everything went well, you should now have a binary called `kernel.bin` in the `build` directory. Congratulations! (If by any chance it didn't go quite that well and instead gcc gave you a bunch of errors, then fix them! It's not my fault you can't write C!)

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

Now, move `kernel.bin` from `build` to `kernel/arch/x86_64/boot`

```
mcopy -i fat.img kernel.bin ::/EFI/BOOT
```

Now, move `fat.img` to `build/iso` and run

```
xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o daphne_img_x64.iso iso
```

from the `build` directory

## Testing x64 images in Virtual box
First, make sure the "Enable EFI (special OSes only)" checkbox under system is checked. Then, once
you point the VM to the ISO image, start the VM. When in the UEFI shell, type this sequence of commands

```
fs1:
```

```
cd EFI/BOOT
```

```
bootx64.efi
```