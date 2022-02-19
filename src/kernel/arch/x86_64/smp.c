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

static struct processor *cpus;
static struct smp_cpus *smp_cores;
static uint64_t lapic_base;
static uint64_t ioapic_base; /* Can there be multiple IOAPICS? Sure. Do I give a shit? Nope. */

static int current_ap = 0;

/**
 * LAPIC base address
 */
static uint64_t apic;

static bool _ap_is_ok = false;

extern uint32_t ap_bootstrap16;
extern uint32_t ap_bootstrap_end;

static struct madt_table_ioapic **ioapic_table;

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

/* LAPIC functions */
void lapic_write(uint32_t reg, uint32_t val)
{
    *((volatile uint32_t *)(((uintptr_t) apic) + reg)) = val;
    asm volatile ("":::"memory");
}

uint32_t lapic_read(uint32_t reg)
{
    return *((volatile uint32_t *)((uintptr_t) apic + reg));
}

/* Send an EOI to the lapic */
void lapic_eoi(void)
{
    lapic_write(EOI, 0);
}

/* I/O APIC functions */
inline static void ioapic_write(uint32_t reg, uint32_t val)
{
    *(volatile uint32_t *)(ioapic_base) = reg;
    *(volatile uint32_t *)(ioapic_base + 0x10) = val;
}

inline static void ioapic_write64(uint32_t reg, uint64_t val)
{
    uint32_t lo = val & 0xffffffff;
    uint32_t hi = val >> 32;

    ioapic_write(reg, lo);
    ioapic_write(reg + 1, hi);
}

inline static uint32_t ioapic_read(uint32_t reg)
{
    *(volatile uint32_t *)(ioapic_base) = reg;
    return *(volatile uint32_t *)(ioapic_base + 0x10);
}

/**
 * @brief Setup irq for redirect
 */
void lapic_redirect(uint8_t irq, uint8_t vector, uint32_t delivery)
{
    ioapic_write64(IOAPICREDTBL(irq), delivery | vector);
}

/**
 * @brief Initialise the APIC
 *
 * Disables the PIC & sets up the APIC
 * for MMIO.
 */
void lapic_init(void)
{
    if (!cpu_has_apic()) panic("apic not supported!");
    apic = madt_get_header()->lapic;

    /* Get IOAPIC info from MADT */
    ioapic_table = (struct madt_table_ioapic **) madt_get_tables(MADT_IOAPIC);
    ioapic_base = ioapic_table[lapic_read(LAPIC_ID) >> 24]->ioapic_addr;

    /* Map APIC base for MMIO */
    mmu_map_mmio(apic, 3);
    mmu_map_mmio(ioapic_base, 3);

    wrmsr(APIC, (rdmsr(APIC) | 0x800) & ~(LAPIC_ENABLE));

    /* Disable the PIC & enable the APIC */
    lapic_write(SIVR, lapic_read(SIVR) | 0x1ff);
    pic_disable();

    struct madt_table_iso **iso = (struct madt_table_iso **) madt_get_tables(MADT_ISO);
    for (int i = 0; iso[i]; i++) {
        lapic_redirect(iso[i]->gsi, iso[i]->irq + 0x20, 0);
    }
}

/**
 * @brief Send INIT & SIPI
 *
 * Sends IPIs to a CPU
 */
static void lapic_send_init(uint32_t cpu_id)
{
    lapic_write(ICR2, (uint64_t) cpu_id << LAPIC_ICR_CPUID_OFFSET);
    lapic_write(ICR1, LAPIC_ICR_DEST_INIT);
}

static void lapic_send_sipi(uint32_t cpu_id, uintptr_t entry)
{
    lapic_write(ICR2, (uint64_t) cpu_id << LAPIC_ICR_CPUID_OFFSET);
    lapic_write(ICR1, LAPIC_ICR_DEST_SEND_IPI | (entry / 4096));
}

void smp_next_ap(void)
{
    _ap_is_ok = true;
}

int smp_get_current_ap(void)
{
    return current_ap;
}

uint64_t smp_get_lapic_base(void)
{
    return apic;
}

void smp_signal_ap(uint32_t lapic)
{
    ++current_ap;
    lapic_send_init(lapic);
    tsc_delay(5000UL);
    lapic_send_sipi(lapic, AP_BOOTSTRAP_VIRT_START);
}

void smp_init(void)
{
    lapic_init();
    cpus = kmalloc(sizeof(struct processor) * SMP_MAX_CPUS);
    /* Get info from MADT */
    struct madt_table_lapic **m = (struct madt_table_lapic **) madt_get_tables(MADT_LAPIC);
    lapic_base = madt_get_header()->lapic;
    int cores = 0;
    /* Enumerate all cores */
    for (; m[cores]; cores++) {
        /* Too many? Only use SMP_MAX_CPUS */
        if (cores > SMP_MAX_CPUS) {
            warn("smp: too many cpus, defaulting to %ui", SMP_MAX_CPUS);
            break;
        }
        cpus[cores].cpu_id = m[cores]->processor_id;
        cpus[cores].lapic_id = m[cores]->apic_id;
        cpus[cores].task = NULL;
    }

    smp_cores = kmalloc(sizeof(smp_cores));
    smp_cores->cpus = cpus;
    smp_cores->size = cores;

    /* I like my memory free! */
    kfree(m);

    /* No SMP needed? */
    if (cores <= 1) return;

    /* Map lapic for MMIO */
    mmu_map_mmio(lapic_base, 1);

    for (int i = 0; i < cores; i++) {
        const uint32_t bsp_id = (lapic_read(LAPIC_ID) >> 24);
        /*
         * Don't put the BSP in real mode! 
         *
         * Note! I don't know if this check is necessary or if
         * the BSP always has id 0x0, but this check doesn't really
         * impact anything so I'll just leave it here for now
         */
        if ((uint32_t) cpus[i].cpu_id == bsp_id) {
            cpu_set_current_core((uintptr_t)&(smp_cores->cpus[bsp_id]));
            continue;
        };
        uint64_t ap_bootstrap_len = (uintptr_t) &ap_bootstrap_end - (uintptr_t) &ap_bootstrap16;
        _ap_is_ok = false;

        /**
         * Map the kernel's page table, a new stack
         * and the entry point for the AP just below the
         * bootstrap code where the AP can retrieve them
         */
        mmu_map_mmio(0x0, 1);
        uint8_t *cpu_stack = kmalloc(KERNEL_STACK_SIZE * sizeof(uint8_t));
        *((volatile uint64_t *)(SMP_PAGE_TABLE)) = (uint64_t) vmm_get_pml4();
        *((volatile uint64_t *)(SMP_STACK)) = ((uint64_t) cpu_stack + KERNEL_STACK_SIZE);
        *((volatile uint64_t *)(AP_ENTRY)) = ((uintptr_t) ap_startup);

        /* Store GDT & IDT for AP */
        asm volatile(
            "sgdt 0x770\n"
            "sidt 0x790\n"
        );

        /* Map & load bootstrap code */
        mmu_map_mmio(AP_BOOTSTRAP_VIRT_START, (ap_bootstrap_len / PAGE_SIZE) + 2);
        memcpy((void *) AP_BOOTSTRAP_VIRT_START, (void *) &ap_bootstrap16, ap_bootstrap_len);

        /* Startup CPU */
        smp_signal_ap(cpus[i].lapic_id);

        /* Wait for AP to do its thing before continuing */
        do { 
            asm volatile ("pause" : : : "memory");
        } while (!_ap_is_ok);
    }

    ok("smp: initialised SMP with %ui cores", cores);
}

struct smp_cpus *smp_get_cores(void)
{
    return smp_cores;
}
