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

#include <arch/x86_64/acpi.h>
#include <arch/x86_64/vmm.h>

#include <generic/malloc/malloc.h>

enum madt_type {
    MADT_LAPIC = 0,
    MADT_IOAPIC = 1,
    MADT_ISO = 2,
    MADT_NMI = 4,
};

struct madt_record_table_entry {
    uint8_t type;
    uint8_t length;
} __attribute__((packed));

struct madt_table_lapic {
    struct madt_record_table_entry std;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t misc_flag;
} __attribute__((packed));

struct madt_table_ioapic {
    struct madt_record_table_entry std;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_addr;
    uint32_t gsib;
} __attribute__((packed));

struct madt_table_iso {
    struct madt_record_table_entry std;
    uint8_t bus;
    uint8_t irq;
    uint32_t interrupt;
    uint16_t flags;
} __attribute__((packed));

struct madt_table_nmi {
    struct madt_record_table_entry std;
    uint8_t processor_id;
    uint16_t flags;
    uint8_t lintno;
} __attribute__((packed));

struct madt_head {
    struct rsdt_header head;
    uint32_t lapic;
    uint32_t flags;
    struct madt_record_table_entry madt_table[];
} __attribute__((packed));

/**
 * madt_init
 *   brief: init madt
 */
void madt_init(void);

/**
 * madt_length
 *   brief: get madt length
 */
uint64_t madt_length(void);

/**
 * madt_get_tables
 *   brief: get tables of certain madt_type
 */
void **madt_get_tables(enum madt_type type);

/**
 * madt_get_header
 *   brief: get madt header
 */
struct madt_head *madt_get_header(void);
