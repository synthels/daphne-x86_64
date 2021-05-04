# phiOS
Just another hobby operating system!
Check it out over at my [YouTube channel](https://www.youtube.com/channel/UCzXuqt4_pAogDUzy3RpN9nQ), I guess?

# Building from source
The only dependency for building phiOS is the `i686-elf-*` GCC toolchain and a UNIX-like environment. All you have to do is run `build.sh` in order to get a multiboot binary that is sufficient for testing in QEMU. If you want a working ISO, you can run `build.sh iso` which will spit out an ISO, with the only extra dependency being `grub-mkrescue`. You will also find official builds in the releases tab on github.
