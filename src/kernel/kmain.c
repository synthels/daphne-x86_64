/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
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
 * main function
 */

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

#include <generic/forbia/kernel.h>
#include <generic/io/io.h>
#include <generic/memory/mem.h>
#include <generic/malloc/malloc.h>
#include <generic/shrimp/shrimp.h>
#include <generic/time/time.h>
#include <generic/time/sleep.h>
#include <generic/sched/task.h>

#include <mod/fb/lfb.h>
#include <mod/pci/pci.h>
#include <mod/hpet/hpet.h>
#include <mod/kbd/kbd.h>

#include <lib/printk.h>

#include <tests/test.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/acpi.h>
    #include <arch/x86_64/madt.h>
    #include <arch/x86_64/smp.h>
#endif

#include "kmain.h"

/* Kernel stack */
static uint8_t stack[KERNEL_STACK_SIZE] __attribute__((aligned(16))) = {0};

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
    /* Get RSDP */
    struct stivale2_struct_tag_rsdp *rsdp_info = get_tag(stv, STIVALE2_STRUCT_TAG_RSDP_ID);

    gdt_init(); /* gdt & tss */
    idt_init(); /* idt */
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

    #ifdef BUILD_TESTS
        /* Only give test output if tests are built
           with the kernel */
        set_log_level(TEST);
    #endif

    info("daphne (forbia %s, running on %s)", KERNEL_VERSION, KERNEL_ARCH);
    info("display info - width: %i, height: %i, pitch: %i", 
        info.screen_width,
        info.screen_height,
        info.screen_pitch
    );
    ok("initialized terminal with printk_buffer_size=%i, log_level=%i", __PRINTK_BUFFER_SIZE, get_log_level());

    time_init();                    /* time */
    pci_scan();                     /* pci */

    #ifdef ARCH_x86_64
        acpi_init(rsdp_info->rsdp);           /* acpi */
        madt_init();                          /* madt */
        smp_init();                           /* smp */
        hpet_init(time_get_root_func_node()); /* try to boot up HPET */
    #endif

    enable_interrupts(); /* enable interrupts right when the APIC kicks in */
    kbd_init();          /* init ps2 keyboard */
    init_sched();        /* init scheduler */

    #ifdef BUILD_TESTS
        /* Run unit tests */
        run_unit_tests();
        log_test("All tests complete!");
    #endif

    for (;;) {
        asm("hlt");
    }
}
