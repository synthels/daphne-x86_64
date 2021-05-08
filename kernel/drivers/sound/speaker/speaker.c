/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * PC Speaker driver
 */

/*
 * The PC Speaker driver expects data to be written to it
 * like this:
 *
 * spk_interface->write(freq, DRIVER_WRITE);
 * spk_interface->write(ticks, DRIVER_WRITE);
 * spk_interface->write(0, DRIVER_COMMIT);
 *
 * Anything else will cause it to throw a tantrum. Please don't
 * allow it to do that.
 */

#include "speaker.h"

static int writes = 0;

/* Speaker data */
static int freq = 250;
static int ticks = 10;

void spk_no_read(uint32_t *data)
{
	*data = DRIVER_NO_READ;
}

void spk_init(void)
{
	spk_interface = (struct driver_interface *) lmmap(sizeof(struct driver_interface));

	spk_interface->enabled = 1;
	spk_interface->event = 0;
	spk_interface->write = &spk_play_freq_tm;
	spk_interface->read = &spk_no_read;
}

/*
 * Play a certain frequency through the PC speaker 
 * for a certain amount of ticks
 */
int spk_play_freq_tm(uint32_t data, int commit)
{
	writes++;
	if (commit == 1) {
		/* Committed with correct amount of data */
		if (writes == 3) {
			writes = 0;
			spk_play_freq(freq);
			sleep(ticks);
			spk_stop();
			return DRIVER_WRITE_OK;
		}

		/* Committed with too little writes */
		if (writes < 2) {
			writes = 0;
			return DRIVER_TOO_LITTLE_WRITES;
		}		
	}

	/* Too many writes */
	if (writes > 2) {
		writes = 0;
		return DRIVER_TOO_MANY_WRITES;
	}

	switch (writes) {
		case 1:
			freq = data;
			return DRIVER_EXPECTING_NEXT_WRITE;
		case 2:
			ticks = data;
			return DRIVER_EXPECTING_NEXT_WRITE;
	}

	DRIVER_WRITE_END;
}

/* Play a certain frequency through the PC speaker */
void spk_play_freq(uint16_t freq)
{
	if (freq == 0) { return; }	

	/* Set frequency */
	uint32_t div = 1193180 / freq;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t) div);
	outb(0x42, (uint8_t) (div >> 8));

	uint8_t tmp = inb(0x61);
	if (tmp != (tmp | 3)) {
		outb(0x61, tmp | 3);
	}
}

/* Stop the speaker */
void spk_stop(void)
{
	outb(0x61, inb(0x61) & 0xfc);
}
