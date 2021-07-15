# Daphne's Event based IO layer
Daphne offers a non POSIX compliant event based IO layer, intended to be used by the Glibc/POSIX compatibility layer. It is very similar in nature to the same interface found in the linux kernel, only with a few key differences. Every function described below can be found in `forbia/eventio.h`

## Polling the kernel for IO events
The kernel exposes the function `bool io_poll(int io_device, struct io_event *event)` for this very purpose. `io_poll` accepts 2 arguments. `io_handle`, which defines the particular IO handle (see [io.md](https://github.com/synthels/daphne/blob/master/docs/io.md)) that should be polled, and `event` which is a pointer to an `io_event` structure, that describes an IO event. Its structure is as follows

```c
struct io_event {
    int io_handle;
};
```

The `type` field can take the following values

The event structure pointed at by `io_poll` can then be consumed like so

```c
#include <forbia/eventio.h>
#include <forbia/io.h>

int main(void)
{
    struct file *f = fopen("file");
    struct io_event event;
    while(true) {
        /* Poll the file */
        if (io_poll(f.handle, &event)) {
            printk("File has updated");
        }
    }
}
```
