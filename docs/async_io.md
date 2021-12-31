# Daphne's Async IO layer

Daphne offers a non POSIX compliant async based IO layer, intended to be used by the Glibc/POSIX compatibility layer. It is very similar in nature to the same interface found in the linux kernel, only with a few key differences. Every function described below can be found in `forbia/async.h`

## Polling the kernel for IO events

The kernel exposes the function `bool io_poll(int io_device, struct io_event *event)` for this very purpose. `io_poll` accepts 2 arguments. `io_handle`, which defines the particular IO handle (see [io.md](https://github.com/synthels/daphne/blob/master/docs/io.md)) that should be polled, and `event` which is a pointer to an `io_event` structure, that describes an IO event. Its structure is as follows

```c
struct io_event {
    int io_handle;
    int type;
};
```

The `type` field can take the following values

The event structure pointed at by `io_poll` can then be consumed like so

```c
#include <generic/forbia/async.h>
#include <generic/forbia/io.h>
#include <generic/forbia/types.h>

int main(void)
{
    struct file *f = fopen("file");
    struct io_event event;
    async_fwrite(f, "Async writes!", true);
    while (true) {
        /* Poll the file */
        if (io_poll(f.handle, &event)) {
            switch (event.type) {
                case ASYNC_WRITE_DONE:
                    printk("File write done!");
                default:
                    printk("File write not done yet!");
            }
        }
    }
}
```

As you saw in the above example, `forbia/async.h` also exposes `async_fwrite`. The truth is, it exposes an async counterpart for every synchronous IO function. These are the functions:

- `struct file *async_fopen(const char *path)`
- `int async_fwrite(struct file *f, const char *str, int overwrite)`
- `void async_fseek(int position)` (Note: will set `ASYNC_OPERATION_DONE`)
- `void async_fclose(struct file *f)`
- `void async_fread(void *ptr, size_t size, struct file *f)`

The enum that encapsulates all event types is defined as follows:

```c
enum async_io_types {
    ASYNC_WRITE_DONE        = 0,
    ASYNC_READ_DONE         = 1,
    ASYNC_OPERATION_DONE    = 2,
    ASYNC_OPEN_DONE         = 3
    ASYNC_CLOSE_DONE        = 4
};
```
