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

Before building, you must also install the `i686-elf-gcc` compiler. The best way to get it is to use brew like so

```
brew install i686-elf-gcc
```

### WSL note
On WSL, you may have to run these 2 commands every time you start up the shell, otherwise CMake will not be able to find the compiler

```
export BREW_HOME="/home/linuxbrew/.linuxbrew/bin"
```

```
export PATH="$PATH:$BREW_HOME"
```

# Let's start the build!
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

# Building an ISO image
If you're still here, you might be interested in building an ISO image. Lucky for you, I can tell you how to do just that!

First of all, run the following command to strip any debug info off the kernel binary
```
strip --strip-debug kernel.bin
```

Then, move `kernel.bin` to the `iso/boot` directory.

Next, to build the ISO, just run
```
grub-mkrescue -o daphne-img-grub.iso iso
```
