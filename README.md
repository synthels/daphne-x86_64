![Where is the logo?](https://github.com/synthels/phiOS/blob/master/images/logo.png)
# phiOS
A non-UNIX x86-32 (and hopefully x86-64 in the future) operating system just like all others.

# Building from source
## Building the toolchain
The only dependency for building phiOS is the `i686-elf-*` GCC toolchain and a UNIX-like environment. To build this toolchain from source, run `./scripts/toolchain.sh`.

## Building the kernel
The kernel uses CMake as its build system. To build the kernel, you must first run `./scripts/setup.sh -mk-grub`. After the `build` directory is created, follow these steps:

```
cd build
```

```
cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=i686-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_32 ..
```

If by any chance you want to build with testing enabled, use this command instead

```
cmake -G "Unix Makefiles" -DBUILD_TESTS=all -DCMAKE_C_COMPILER=i686-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_32 ..
```

Then, to build the binary run

```
cmake --build .
```

After all that, you should have a `kernel.bin` in the `build` directory. Together with the `boot` directory, you should be able to compile an ISO with `grub-mkrescue -o phiOS.iso iso` (After moving `kernel.bin` to the `iso/boot` directory). This has been tested on both Windows (under MinGW) and Linux.

In order to clean the `build` directory, you can run the `clean.sh` script.

## Testing the kernel
The kernel binary that CMake spits out is multiboot compliant, which means that it can be ran in QEMU using the `-kernel` option.

# Contribute
In order to contribute code, you must open a pull request. See `CONTRIBUTING.md` before doing anything.
