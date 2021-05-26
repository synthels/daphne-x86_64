![Where is the logo?](https://github.com/synthels/phiOS/blob/master/images/logo.png)
# <p style="text-align:center"> phiOS </p>
A non-UNIX x86-32 (and hopefully x86-64 in the future) operating system just like all others.

# Building from source
## Building the toolchain
The only dependency for building phiOS is the `i686-elf-*` GCC toolchain and a UNIX-like environment. To build this toolchain from source, run `./scripts/toolchain.sh`.

## Building the kernel
The kernel uses CMake as its build system. To build the kernel, you must first run `./scripts/setup.sh`. After the `build` directory is created, follow these steps:

```
cd build
```

```
cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=i686-elf-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/x86.cmake -DARCH=x86_32 ..
```

```
cmake --build .
```

After all that, you should have a `kernel.bin` in the `build` directory. Together with the `boot` directory, you should be able to compile an ISO with `grub-mkrescue -o phiOS.iso iso`. This has been tested on both Windows (under MinGW) and Linux.

# Contribute
In order to contribute code, you must open a pull request. See `CONTRIBUTING.md` before doing anything.
