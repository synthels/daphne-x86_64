#ifndef DRIVER
#define DRIVER

#include <stdint.h>
#include <kernel.h>

#define DRIVER_NO_WRITE -1

struct driver_interface {
	char		*name;
	int			(*write)	(uint8_t *);
	void		(*read)		(uint8_t *);
	int			enabled;
	int			event; /* Set if an event is raised (SHOULD ONLY BE SET BY THE DRIVER & CLEARED BY THE KERNEL) */
};

#endif
