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

static uint64_t apic;
static struct processor *cpus;
static uint64_t lapic_base;

static bool _ap_startup_flag = false;

extern uint32_t ap_bootstrap16;
extern uint32_t ap_bootstrap_end;

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

/**
 * @brief Initialise the APIC
 *
 * Disables the PIC & sets up the APIC
 * for MMIO.
 */
static void apic_init(void)
{
    if (!cpu_has_apic()) panic("apic not supported!");
    apic = madt_get_header()->lapic;
    /* Map APIC base for MMIO */
    mmu_map_mmio(apic, 3);

    wrmsr(APIC, (rdmsr(APIC) | 0x800) & ~(LAPIC_ENABLE));

    /* Disable the PIC & enable the APIC */
    apic_write(SIVR, apic_read(SIVR) | 0x1ff);
    pic_disable();
}

/**
 * @brief Send an IPI
 *
 * @param int Interrupt
 * @param flags Flags
 */
static void apic_send_ipi(int intr, uint32_t flags)
{
	apic_write(0x310, intr << 24);
	apic_write(0x300, flags);
	do { asm volatile ("pause" : : : "memory"); } while (apic_read(0x300) & (1 << 12));
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
    _ap_startup_flag = true;
    for (;;) {
        asm("hlt");
    }
}

/* TODO: delay */
void smp_signal_ap(uint32_t lapic)
{
    /* INIT */
    apic_send_ipi(lapic, 0x4500);
    /* SIPI */
    apic_send_ipi(lapic, 0x4601);
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
        cpus[cores].lapic_id = m[cores]->apic_id;
        cpus[cores].task = NULL;
    }

    /* I like my memory free! */
    kfree(m);

    /* No SMP needed? */
    if (cores <= 1) return;

    /* Map lapic for MMIO */
    mmu_map_mmio(lapic_base, 1);

    for (int i = 0; i < cores; i++) {
        /*
         * Don't put the BSP in real mode! 
         *
         * Note! I don't know if this check is necessary or if
         * the BSP always has id 0x0, but this check doesn't really
         * impact anything so I'll just leave it here for now
         */
        if ((uint32_t) cpus[i].cpu_id == (apic_read(LAPIC_ID) >> 24)) continue;
        uint64_t ap_bootstrap_len = (uintptr_t) &ap_bootstrap_end - (uintptr_t) &ap_bootstrap16;

        _ap_startup_flag = false;

        /**
         * We drop cr3 at the address SMP_PAGE_TABLE, from
         * where every AP can retrieve it. We also drop
         * a new stack at SMP_STACK for the AP. (see smp.asm)
         */
        mmu_map_mmio(SMP_PAGE_TABLE, 1);
        uint8_t *cpu_stack = kmalloc(KERNEL_STACK_SIZE * sizeof(uint8_t));
        *((volatile uint64_t *)(SMP_PAGE_TABLE)) = cpu_get_cr3();
        *((volatile uint64_t *)(SMP_STACK)) = ((uint64_t) cpu_stack + KERNEL_STACK_SIZE);

        /* Store GDT & IDT for AP */
        asm volatile(
            "sgdt 0x770\n"
            "sidt 0x790\n"
        );

        /* Map & load trampoline */
        mmu_map_mmio(AP_BOOTSTRAP_VIRT_START, (ap_bootstrap_len / PAGE_SIZE) + 2);
        memcpy((void *) AP_BOOTSTRAP_VIRT_START, (void *) &ap_bootstrap16, ap_bootstrap_len);

        /* Startup CPU */
        smp_signal_ap(cpus[i].lapic_id);
        ok("smp: loaded cpu 0x%x", cpus[i].cpu_id);

        /* Wait for AP to do its thing before continuing */
        do { 
            asm volatile ("pause" : : : "memory");
        } while (!_ap_startup_flag);
    }
}
