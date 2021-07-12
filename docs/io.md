# Daphne's low level IO library
Daphne, like any operating system should, provides a low level file system agnostic IO layer. It can be found under `forbia/io.h`.

## Accessing files
A file is described using the `file` structure. It is defined as follows

```c
struct file {
    int handle;
    int position;
    struct file_location location; /* Undefined - Handled internally by the kernel */
};
```

A file can be opened with the `file *fopen(const char *path)` function. It accepts a path on the file system (see [fs.md](https://github.com/synthels/daphne/docs/fs.md)). At this level, `fopen` does not care about modes. All files opened are both readable & writable. `fopen` does acquire a spin lock from the kernel, so that no two processes can open the same file at the same time.

## File IO
Doing file IO at this level is simple. `int fwrite(file *f, const char *str, int overwrite)` erases the whole file and then writes the string `str` to it if overwrite is set. If not, it merely appends `str` to the file. The current position in the file can be set with the `void fseek(int position)` function. This function overwrites the `position` field in the file structure and may do more internal magic in order to decrease cold `fwrite`s. `fseek(0)` is always the beginning of the file, and `fseek(fgetsize(f) - 1)` is the end. `int fgetsize(file *f)` returns the size of the file `f` in bytes (octets). Finally, `void fclose(file *f)` `free`s the file pointer and releases its spin lock.

## IO handles
IO handles are random signed integers bound to specific IO resources that are generated at the time `fopen` is called. They can and will be extended to other IO resources such as sockets in the future.
