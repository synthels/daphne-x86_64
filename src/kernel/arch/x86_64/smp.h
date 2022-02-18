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
 */

#pragma once

#include <arch/x86_64/madt.h>
#include <arch/x86_64/vmm.h>
#include <arch/x86_64/pic.h>
#include <arch/x86_64/tsc.h>

#include <lib/printk.h>

#include <generic/sched/task.h>
#include <generic/malloc/malloc.h>
#include <generic/forbia/types.h>
#include <generic/forbia/types.h>

/* ap_startup */
#include <kmain.h>

#define SMP_MAX_CPUS 32

#define LAPIC_ENABLE (1 << 10)
#define LAPIC_ICR_CPUID_OFFSET 24
#define LAPIC_ICR_DEST_INIT (5 << 8)
#define LAPIC_ICR_DEST_SEND_IPI (6 << 8)

#define AP_BOOTSTRAP_VIRT_START 0x1000

#define SMP_PAGE_TABLE 0x700
#define SMP_STACK 0x750
#define AP_ENTRY 0x800

#define IOAPICREDTBL(n) (0x10 + 2 * n)
#define ICR_MESSAGE_TYPE_LOW_PRIORITY (1 << 8)

enum SMP_MSR {
    APIC = 0x1b,
    EFER = 0xc0000080,
    STAR = 0xc0000081,
    LSTAR = 0xc0000082,
    COMPAT_STAR = 0xc0000083,
    SYSCALL_FLAG_MASK = 0xc0000084,
    FS_BASE = 0xc0000100,
    GS_BASE = 0xc0000101,
    KERN_GS_BASE = 0xc0000102
};

enum SMP_APIC_REGS {
    LAPIC_ID = 0x20,
    EOI = 0xb0,
    SIVR = 0xf0,
    ICR1 = 0x300,
    ICR2 = 0x310,
    LVT_TIMER = 0x320,
    LINT1 = 0x350,
    LINT2 = 0x360,
    TIMER_DIV = 0x3e0,
    TIC = 0x380,
    TCURR = 0x390
};

struct smp_cpus {
    struct processor *cpus;
    size_t size;
};

/**
 * smp_init
 *   brief: init smp
 */
void smp_init(void);

/**
 * smp_next_ap
 *   brief: sets up next AP in SMP startup
 */
void smp_next_ap(void);

/**
 * smp_get_cores
 *   brief: get processor struct
 */
struct smp_cpus *smp_get_cores(void);

/**
 * smp_get_current_ap
 *   brief: get current ap
 */
int smp_get_current_ap(void);

/**
 * lapic_init
 *   brief: init lapic
 */
void lapic_init(void);

/**
 * lapic_write
 *   brief: write to the lapic
 */
void lapic_write(uint32_t reg, uint32_t val);

/**
 * lapic_read
 *   brief: read from the lapic
 */
uint32_t lapic_read(uint32_t reg);

/**
 * lapic_redirect
 *   brief: Setup irq redirect
 */
void lapic_redirect(uint8_t irq, uint8_t vector, uint32_t delivery);

/**
 * lapic_eoi
 *   brief: send an EOI to the lapic
 */
void lapic_eoi(void);
