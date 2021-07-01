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
First, you have a choice to build one of two bootloaders. You can either build limine, or, the currently very broken tulip. 


## Building tulip (please don't)
run `make` in `kernel/arch/x86_64/tulip`.

## Building limine
building limine is also really simple. All you have to do is run `make` from inside the `kernel/arch/x86_64/limine` directory.

## Building the kernel
First, run

```
./tools/setup.sh -x64
```

from the root directory

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

## x86_64 UEFI
In order to build an ISO image under x86_64, follow these instructions.
First, `cd` to `kernel/arch/x86_64/limine/bin`. Then, run the following commands

```
cp -v limine.sys limine-cd.bin limine-eltorito-efi.bin ../../../../../build/iso/
```

Now, `cd` to `build`

```
cp -v kernel.bin iso
```

```
xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-eltorito-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso -o daphne_img_x64.iso
```

if you also want BIOS, you might want to run these commands

```
cd ../kernel/arch/x86_64/limine/bin
```

```
./limine-install ../../../../../build/daphne_img_x64.iso
```

## Testing x64 images in Virtualbox
First, make sure the "Enable EFI (special OSes only)" checkbox under system is checked. Then, once
you point the VM to the ISO image, start the VM. When in the UEFI shell, type this sequence of commands

```
fs1:
```

```
cd EFI/BOOT
```

```
BOOTX64.EFI
```

## Testing x64 images in QEMU
Testing under QEMU requires that you have an OVMF image installed, then, at least under linux, you can use
this command

```
qemu-system-x86_64 --bios /usr/share/qemu/OVMF.fd -m 4096 -cdrom daphne_img_x64.iso -vga virtio
```
