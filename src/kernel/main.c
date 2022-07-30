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

#include <generic/forbia/kernel.h>
#include <generic/memory/mem.h>
#include <generic/malloc/malloc.h>
#include <generic/shrimp/shrimp.h>
#include <generic/time/clock.h>
#include <generic/time/time.h>
#include <generic/time/sleep.h>
#include <generic/sched/exec.h>
#include <generic/fs/vfs.h>
#include <generic/forbia/stivale2.h>

#include <modules/ramdisk.h>
#include <modules/fb/lfb.h>
#include <modules/pci/pci.h>
#include <modules/apic/apic.h>
#include <modules/kbd/kbd.h>

#include <lib/log.h>
#include <lib/string.h>
#include <lib/stdlib.h>

#include <tests/test.h>

#ifdef ARCH_x86_64
    #include <arch/x86_64/acpi.h>
    #include <arch/x86_64/madt.h>
    #include <arch/x86_64/tsc.h>
    #include <arch/x86_64/smp.h>
    #include <arch/x86_64/ports.h>
    #include <arch/x86_64/gdt.h>
    #include <arch/x86_64/vmm.h>
    #include <arch/x86_64/idt/idt.h>
#endif

#include "main.h"

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

/**
 * @brief Startup routine for APs
 *
 * The AP must unset the _ap_startup_flag
 * so that the next CPU can receive the INIT. The
 * startup routine then puts the AP in a usable
 * state
 */
void ap_startup(void)
{
    const struct processor *cpu = smp_get_cores()->cpus[smp_get_current_ap()];
    cpu_set_current_core((uintptr_t) cpu);
    smp_next_ap();
    idt_init();
    ap_lapic_init();
    apic_init();
    enable_interrupts();

    for (;;) {
        asm("hlt");
    }
}

/**
 * @brief Kernel main
 *
 * Sets up all x86 stuff (TODO: it sould be separate, anyways...)
 * and everything needed to eventually boot into
 * a usable userspace
 */
void main(struct stivale2_struct *stv)
{
    /* Get memory map */
    struct stivale2_struct_tag_memmap *mmap = get_tag(stv, STIVALE2_STRUCT_TAG_MEMMAP_ID); /* TODO: Handle NULL */
    /* Get framebuffer info */
    struct stivale2_struct_tag_framebuffer *fb_info = get_tag(stv, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    /* Get RSDP */
    struct stivale2_struct_tag_rsdp *rsdp_info = get_tag(stv, STIVALE2_STRUCT_TAG_RSDP_ID);
    /* Get module info */
    struct stivale2_struct_tag_modules *mod_info = get_tag(stv, STIVALE2_STRUCT_TAG_MODULES_ID);

    #ifdef ARCH_x86_64
        gdt_init(); /* gdt & tss */
        idt_init(); /* idt */
    #endif

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

    #ifdef ARCH_x86_64
        pci_scan();                    /* pci */
        acpi_init(rsdp_info->rsdp);    /* acpi */
        madt_init();                   /* madt */
    #endif

    enable_interrupts();               /* enable interrupts */
    kbd_init();                        /* init ps2 keyboard */
    clock_init();                      /* clock */
    time_init();                       /* time */
    smp_init();                        /* smp */
    tsc_init();                        /* tsc */
    apic_init();                       /* try to boot up the APIC timer */
    sched_init();                      /* scheduler */
    vfs_init();                        /* vfs */

    /* Try to find ramdisk */
    struct stivale2_module *rd = NULL;
    for (size_t i = 0; i < mod_info->module_count; i++) {
        struct stivale2_module *m = &(mod_info->modules[i]);
        if (!strcmp(m->string, "ramdisk")) {
            ramdisk_init((void *) m->begin);
            rd = m;
        }
    }

    /* Where's the ramdisk?? */
    if (!rd) panic("ramdisk not found on boot device!");

    #ifdef BUILD_TESTS
        /* Run unit tests */
        run_unit_tests();
        pr_test("All tests complete!");
    #endif

    for (;;) {
        asm("hlt");
    }
}
