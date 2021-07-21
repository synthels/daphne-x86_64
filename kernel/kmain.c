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

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

#include "kmain.h"

/* Kernel stack */
static uint8_t stack[STACK_SIZE] __attribute__((aligned(16))) = {0};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t) stack + sizeof(stack),
    .flags = (1 << 1),
    .tags = (uintptr_t) &framebuffer_hdr_tag
};

void *get_tag(struct stivale2_struct *stv, uint64_t id)
{
    struct stivale2_tag *tag = (void *) stv->tags;
    for (;;) {
        if (tag == NULL)
            return tag;
        if (tag->identifier == id)
            return tag;
        tag = (void *) tag->next;
    }
}

void kmain(struct stivale2_struct *stv)
{
    /* Get memory map */
    struct stivale2_struct_tag_memmap *mmap = get_tag(stv, STIVALE2_STRUCT_TAG_MEMMAP_ID); /* TODO: Handle NULL */
    /* Get framebuffer info */
    struct stivale2_struct_tag_framebuffer *fb_info = get_tag(stv, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    init_gdt(); /* gdt & tss */
    init_idt(); /* idt */
    mem_init(mmap->memmap, mmap->entries); /* mm */
    vmm_init(); /* vmm */
    dev_init(); /* essential devices */
    lfb_init(fb_info->framebuffer_width, fb_info->framebuffer_height, fb_info->framebuffer_addr, fb_info->framebuffer_pitch); /* video */

    /* Create kernel video context */
    struct gfx_context kern_ctx;
    struct lfb_info info;
    struct pos ctx_pos = {0, 0};
    lfb_get_info(&info);
    lfb_create_ctx(&kern_ctx, ctx_pos, info.screen_width, info.screen_height);

    /* Initialize fbterm with kernel handle */
    shrimp_init(kern_ctx.handle);
    printk("daphne (forbia %s, running on %s)\n", KERNEL_VERSION_STRING, KERNEL_ARCH_STRING);

    for (;;) {
        asm("hlt");
    }
}
