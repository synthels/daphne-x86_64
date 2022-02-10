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
#include <arch/x86_64/cmos.h>

#include <lib/printk.h>

#include <generic/sched/task.h>
#include <generic/malloc/malloc.h>
#include <generic/forbia/types.h>

#define SMP_MAX_CPUS 32

#define LAPIC_ENABLE (1 << 10)
#define LAPIC_ICR_CPUID_OFFSET 24
#define LAPIC_ICR_DEST_INIT (5 << 8)
#define LAPIC_ICR_DEST_SEND_IPI (6 << 8)

#define AP_BOOTSTRAP_VIRT_START 0x1000

#define SMP_PAGE_TABLE 0x700
#define SMP_STACK 0x750
#define AP_ENTRY 0x800

enum SMP_MSR {
    APIC = 0x1b,
    EFER = 0xc0000080,
    STAR = 0xc0000081,
    LSTAR = 0xc0000082,
    COMPAT_STAR = 0xc0000083,
    SYSCALL_FLAG_MASK = 0xc0000084,
    FS_BASE = 0xc0000100,
    GS_BASE = 0xc0000101,
    KERN_GS_BASE = 0xc0000102,
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
    TIMER_DIV = 0x3E0,
    TIC = 0x380,
    TCURR = 0x390
};

/**
 * smp_init
 *   brief: init smp
 */
void smp_init(void);
