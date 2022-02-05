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

#include <lib/string.h>
#include <generic/forbia/types.h>
#include <lib/printk.h>

struct rsdp_desc {
    char signature[8];
    uint8_t checksum;
    char oem[6];
    uint8_t rev;
    uint32_t rsdt;
    uint32_t len;
    uint64_t xsdt;
    uint8_t ext_checksum;
    uint8_t res[3];
} __attribute__((packed));

struct rsdt_header {
    char signature[4];
    uint32_t len;
    uint8_t rev;
    uint8_t checksum;
    char oem[6];
    char oem_table_id[8];
    uint32_t oemrev;
    uint32_t creator_id;
    uint32_t creator_rev;
} __attribute__((packed));

struct rsdt_desc {
    struct rsdt_header header;
    uint32_t sdt_ptr[];
} __attribute__((packed));

/**
 * acpi_init
 *   brief: init acpi
 */
void acpi_init(uint64_t rsdp);

/**
 * acpi_locate_entry
 *   brief: get RSDT entry with signature sig
 */
void *acpi_get_entry(const char *sig);
