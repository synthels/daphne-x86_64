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
 * Symmetric Multiprocessing
 */

#include "smp.h"

#include <lib/sleep.h>

static uint64_t apic;
static struct processor *cpus;
static uint64_t lapic_base;

// __attribute__((used))
// __attribute__((naked))
// static void __ap_bootstrap(void) {
// 	asm volatile (
//     ".code16\n"
//     ".org 0x0\n"
//     ".global _ap_bootstrap_start\n"
//     "_ap_bootstrap_start:\n"

//     /* Enable PAE, paging */
//     "mov $0xA0, %%eax\n"
//     "mov %%eax, %%cr4\n"

//     /* Kernel base PML4 */
//     ".global init_page_region\n"
//     "mov $init_page_region, %%edx\n"
//     "mov %%edx, %%cr3\n"

//     /* Set LME */
//     "mov $0xc0000080, %%ecx\n"
//     "rdmsr\n"
//     "or $0x100, %%eax\n"
//     "wrmsr\n"

//     /* Enable long mode */
//     "mov $0x80000011, %%ebx\n"
//     "mov  %%ebx, %%cr0\n"

//     /* Set up basic GDT */
//     "addr32 lgdtl %%cs:_ap_bootstrap_gdtp-_ap_bootstrap_start\n"

//     /* Jump... */
//     "data32 jmp $0x08,$ap_premain\n"

//     ".global _ap_bootstrap_gdtp\n"
//     ".align 16\n"
//     "_ap_bootstrap_gdtp:\n"
//     ".word 0\n"
//     ".quad 0\n"

//     ".code64\n"
//     ".align 16\n"
//     "ap_premain:\n"
//     "mov $0x10, %%ax\n"
//     "mov %%ax, %%ds\n"
//     "mov %%ax, %%ss\n"
//     "mov $0x2b, %%ax\n"
//     "ltr %%ax\n"
//     ".extern _ap_stack_base\n"
//     "mov _ap_stack_base,%%rsp\n"
//     ".extern ap_main\n"
//     "callq ap_main\n"

//     ".global _ap_bootstrap_end\n"
//     "_ap_bootstrap_end:\n"
//     : : : "memory"
// 	);
// }

/* MSR functions (yes we assume that MSR is supported) */
inline static uintptr_t rdmsr(uintptr_t msr)
{

    uint32_t low, high;
    asm volatile("rdmsr"
                 : "=a"(low), "=d"(high)
                 : "c"(msr));
    return ((uintptr_t) high << 32) | low;
}

inline static void wrmsr(uintptr_t msr, uintptr_t value)
{

    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    asm volatile("wrmsr"
                 :
                 : "c"(msr), "a"(low), "d"(high));
}

/* APIC functions */
inline static void apic_write(uint32_t regs, uint32_t val)
{
    *((volatile uint32_t *)(((uintptr_t) apic) + regs)) = val;
    asm volatile ("":::"memory");
}

inline static uint32_t apic_read(uint32_t regs)
{
    return *((volatile uint32_t *)((uintptr_t) apic + regs));
}

static void apic_init(void)
{
    if (!cpu_has_apic()) panic("apic not supported!");
    apic = madt_get_header()->lapic;
    /* Map APIC base for MMIO */
    mmu_map_mmio(apic, 3);

    wrmsr(APIC, (rdmsr(APIC) | 0x800) & ~(LAPIC_ENABLE));

    /* Disable the PIC */
    apic_write(SIVR, apic_read(SIVR) | 0x1ff);
    pic_disable();
}

void smp_init(void)
{
    apic_init();
    cpus = kmalloc(sizeof(struct processor) * SMP_MAX_CPUS);
    /* Get info from MADT */
    struct madt_table_lapic **m = (struct madt_table_lapic **) madt_get_tables(MADT_LAPIC);
    lapic_base = madt_get_header()->lapic;
    int cores = 0;
    /* Enumerate all cores */
    for (; m[cores]; cores++) {
        /* Too many? Only use SMP_MAX_CPUS */
        if (cores >= SMP_MAX_CPUS) {
            warn("smp: too many cpus, defaulting to %ui", SMP_MAX_CPUS);
            break;
        }
        cpus[cores].cpu_id = m[cores]->processor_id;
        cpus[cores].task = NULL;
    }

    /* I like my memory free! */
    kfree(m);

    /* No SMP needed? */
    if (cores <= 1) return;

    /* Map lapic for MMIO */
    mmu_map_mmio(lapic_base, 1);
}
