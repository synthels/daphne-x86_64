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

#include <forbia/kernel.h>
#include <io/io.h>
#include <memory/mem.h>
#include <malloc/malloc.h>
#include <shrimp/shrimp.h>

#include <mod/fb/lfb.h>
#include <mod/pci/pci.h>
#include <mod/ahci/ahci.h>
#include <mod/tm/tm.h>

#include <sched/task.h>
#include <sched/sched.h>

#include <lib/printk.h>

#include "kmain.h"

/* Kernel stack */
static uint8_t stack[STACK_SIZE * 8] __attribute__((aligned(16))) = {0};

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

void enable_interrupts(void)
{
    asm volatile("sti");
}

void disable_interrupts(void)
{
    asm volatile("cli");
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
    pmm_init(); /* pmm */
    vmm_init(); /* vmm */
    lfb_init(   /* video */
        fb_info->framebuffer_width, 
        fb_info->framebuffer_height,
        fb_info->framebuffer_addr,
        fb_info->framebuffer_pitch
    );

    /* Create kernel video context */
    struct gfx_context kern_ctx;
    struct lfb_info info;
    struct pos ctx_pos = {0, 0};
    lfb_get_info(&info);
    lfb_create_ctx(&kern_ctx, ctx_pos, info.screen_width, info.screen_height);

    /* Initialize fbterm with kernel handle */
    shrimp_init(kern_ctx.handle);
    info("daphne (forbia %s, running on %s)", KERNEL_VERSION, KERNEL_ARCH);
    info("display info - width: %i, height: %i, pitch: %i", 
        info.screen_width,
        info.screen_height,
        info.screen_pitch
    );
    ok("initialized terminal with printk_buffer_size=%i, log_level=%i", __PRINTK_BUFFER_SIZE, get_log_level());

    if (ktask_run("init") < 0) {
        panic("couldn't create init!");
    }

    tm_init();           /* time */
    sched_init();        /* sched */
    enable_interrupts(); /* Houston, we've got interrupts */
    pci_scan();          /* pci */
    ahci_init();         /* achi */

    for (;;) {
        asm("hlt");
    }
}
