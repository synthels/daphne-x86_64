  #ifndef DRIVER
#define DRIVER

#include <stdint.h>
#include <kernel.h>
#include <memory/lmmap/lmmap.h>

#define DRIVER_NO_WRITE -1

struct driver_interface {
	int (*write) (uint32_t *);
	void (*read) (uint32_t *);
	int enabled;
	int event; /* Set if an event is raised (SHOULD ONLY BE SET BY THE DRIVER & CLEARED BY THE KERNEL) */
};

#endif
