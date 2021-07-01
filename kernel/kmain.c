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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * Kernel main function
 */

#include "kmain.h"

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an uninitialised array in .bss.
static uint8_t stack[1024];

// stivale2 uses a linked list of tags for both communicating TO the
// bootloader, or receiving info FROM it. More information about these tags
// is found in the stivale2 specification.

// stivale2 offers a runtime terminal service which can be ditched at any
// time, but it provides an easy way to print out to graphical terminal,
// especially during early boot.
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
	// All tags need to begin with an identifier and a pointer to the next tag.
	.tag = {
		// Identification constant defined in stivale2.h and the specification.
		.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
		// If next is 0, it marks the end of the linked list of header tags.
		.next = 0
	},
	// The terminal header tag possesses a flags field, leave it as 0 for now
	// as it is unused.
	.flags = 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
	.tag = {
		.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
		.next = (uint64_t)&terminal_hdr_tag
	},
	.framebuffer_width  = 0,
	.framebuffer_height = 0,
	.framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
	.entry_point = 0,
	.stack = (uintptr_t)stack + sizeof(stack),
	.flags = (1 << 1),
	.tags = (uintptr_t)&framebuffer_hdr_tag
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
	struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
	for (;;) {
		if (current_tag == NULL) {
			return NULL;
		}

		if (current_tag->identifier == id) {
			return current_tag;
		}

		// Get a pointer to the next tag in the linked list and repeat.
		current_tag = (void *)current_tag->next;
	}
}

// The following will be our kernel's entry point.
void _start(struct stivale2_struct *stivale2_struct) {
	// Let's get the terminal structure tag from the bootloader.
	struct stivale2_struct_tag_terminal *term_str_tag;
	term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

	// Check if the tag was actually found.
	if (term_str_tag == NULL) {
		// It wasn't found, just hang...
		for (;;) {
			asm ("hlt");
		}
	}

	// Let's get the address of the terminal write function.
	void *term_write_ptr = (void *)term_str_tag->term_write;

	// Now, let's assign this pointer to a function pointer which
	// matches the prototype described in the stivale2 specification for
	// the stivale2_term_write function.
	void (*term_write)(const char *string, size_t length) = term_write_ptr;

	// We should now be able to call the above function pointer to print out
	// a simple "Hello World" to screen.
	// term_write("H", 1);

	// We're done, just hang...
	for (;;) {
		asm ("hlt");
	}
}
