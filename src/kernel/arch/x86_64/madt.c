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
 * MADT
 */

#include "madt.h"

struct madt_head *madt;

uint64_t madt_length(void)
{
    return ((uintptr_t) &(madt->head)) + madt->head.len;
}

void madt_init(void)
{
    madt = (struct madt_head *) acpi_get_entry("APIC");
    if (!madt) {
        panic("missing madt header");
    }

    uintptr_t lbase_addr =  madt->lapic;
    lbase_addr = ALIGN_UP(lbase_addr, PAGE_SIZE);

    pml4_map_page(lbase_addr, lbase_addr, FLAGS_READ_WRITE);
    pml4_map_page(lbase_addr + PAGE_SIZE, lbase_addr + PAGE_SIZE, FLAGS_READ_WRITE);
}

void **madt_get_tables(enum madt_type type)
{
    struct madt_record_table_entry *table = madt->madt_table;
    void **mtio = kmalloc(255);
    uint64_t count = 0;
    while ((uintptr_t) table < madt_length()) {
        if (table->type == type) {
            void *buf = table;
            mtio[count] = (void *) ((uint64_t) buf);
            ++count;
        }
        table = (struct madt_record_table_entry *) (((uint64_t) table) + table->length);
    }
    mtio[count] = NULL;
    return mtio;
}

struct madt_head *madt_get_header(void)
{
    return madt;
}
