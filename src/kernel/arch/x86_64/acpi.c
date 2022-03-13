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
 * ACPI
 */

#include "acpi.h"

static struct rsdp_desc *rsdpv;
static struct rsdt_desc *rsdtv;

static bool validate_rsdt(void)
{
    unsigned char sum = 0;
    for (uint32_t i = 0; i < rsdtv->header.len; i++)
        sum += ((char *) &(rsdtv->header))[i];
    return !sum;
}

void acpi_init(uint64_t rsdp)
{
    rsdpv = (struct rsdp_desc *) rsdp;
    rsdtv = (struct rsdt_desc *) (uint64_t) rsdpv->rsdt;

    if (!validate_rsdt()) {
        panic("invalid rsdt checksum");
    }
}

/* WARNING: assumes sig has size 4! */
void *acpi_get_entry(const char *sig)
{
    int entries = (rsdtv->header.len - sizeof(rsdtv->header))/4;
    for (int i = 0; i < entries; i++) {
        if (!(rsdtv->sdt_ptr[i])) continue;
        struct rsdt_header *h = (struct rsdt_header *) (uint64_t) rsdtv->sdt_ptr[i];
        if (!strncmp(h->signature, sig, 4)) return (void *) h;
    }

    return NULL;
}
