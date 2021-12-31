/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
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
 * x64 vmm
 */

#include "vmm.h"

static uint64_t *pml4;
extern uint64_t kernel_end;

uint64_t *next_level_or_alloc(uint64_t *current_level, size_t entry)
{
    if (!(current_level[entry] & 1)) {
        current_level[entry] = (uint64_t) pmm_alloc_page();
        memset((void *) current_level[entry], 0, PAGE_SIZE);
        current_level[entry] |= 3;
    }
    return (uint64_t *)((current_level[entry] & ~(0x1ff)));
}

size_t get_pml_index(uint64_t addr)
{
    return (addr >> 12) & 0x1ff;
}

/* Copy a page from pml src to pml dst */
void copy_page(uint64_t *src, uint64_t *dst, uint64_t addr)
{
    size_t idx = get_pml_index(addr);
    map_page(dst, addr, src[idx], FLAGS_READ_WRITE);
}

/* Map spec regions */
void map_spec(uint64_t *pml)
{
    /* Map first 4 GB */
    for (uintptr_t i = 0; i < 4 * GB; i += PAGE_SIZE) {
        map_page(pml, MEM_PHYS_OFFSET + i, i, FLAGS_READ_WRITE);
        map_page(pml, i, i, FLAGS_READ_WRITE);
    }
    /* Map 0x00000000 -> 0xffffffff80000000 as per the spec */
    for (uint64_t addr = 0; addr < 0x80000000; addr += PAGE_SIZE) {
        map_page(pml, addr + MEM_KERN_OFFSET, addr, FLAGS_READ_WRITE);
    }
}

/* Map kernel space */
void map_kernel(uint64_t *pml, bool load_pml)
{
    map_spec(pml);
    if (load_pml) {
        /* Map kernel heap */
        for (uint64_t i = 0; i < HEAP_PAGES; i++) {
            map_page(pml, KERNEL_HEAP_LOW + (i * PAGE_SIZE), (uint64_t) pmm_alloc_page(), FLAGS_READ_WRITE);
        }
        asm volatile ("movq %0, %%cr3" : : "r"((uint64_t) pml));
    } else {
        uint64_t *swp_pml4 = pmm_alloc_page();
        map_spec(pml);
        /* Copy heap over */
        for (uint64_t i = 0; i < HEAP_PAGES; i++) {
            copy_page(pml4, swp_pml4, KERNEL_HEAP_LOW + (i * PAGE_SIZE));
        }
        pml4 = swp_pml4;
    }
}

void vmm_init(void)
{
    pml4 = pmm_alloc_page();
    memset(pml4, 0, PAGE_SIZE);
    /* Recursive mapping */
    pml4[511] = (uint64_t) pml4;
    map_kernel(pml4, true);
}

void vmm_init_pml(uint64_t *pml)
{
    map_kernel(pml, false);
}

void map_page(uint64_t *pml, uint64_t virt_addr, uint64_t phys_addr, uint64_t flags)
{
    /* Calculate every index */
    size_t pml4_entry = (virt_addr >> 39) & 0x1ff;
    size_t pml3_entry = (virt_addr >> 30) & 0x1ff;
    size_t pml2_entry = (virt_addr >> 21) & 0x1ff;
    size_t pml1_entry = (virt_addr >> 12) & 0x1ff;

    /* Traverse levels */
    uint64_t *pml3 = next_level_or_alloc(pml, pml4_entry);
    uint64_t *pml2 = next_level_or_alloc(pml3, pml3_entry);
    uint64_t *pml1 = next_level_or_alloc(pml2, pml2_entry);

    /* Create mapping */
    pml1[pml1_entry] = (uint64_t) (phys_addr | flags);
}

uint64_t *vmalloc(size_t n)
{
    vmm_init_pml(pml4);
    for (uint64_t i = 0; i < n; i++) {
        map_page(pml4, i * PAGE_SIZE, (uint64_t) pmm_alloc(PAGE_SIZE), FLAGS_READ_WRITE);
    }

    for (uint64_t i = PROC_STACK_LOW; i < PROC_STACK_SIZE; i++) {
        map_page(pml4, i * PAGE_SIZE, (uint64_t) pmm_alloc(PAGE_SIZE), FLAGS_READ_WRITE);
    }

    for (uint64_t i = PROC_HEAP_LOW; i < PROC_HEAP_SIZE; i++) {
        map_page(pml4, i * PAGE_SIZE, (uint64_t) pmm_alloc(PAGE_SIZE), FLAGS_READ_WRITE);
    }

    return pml4;
}

// uint64_t *vfree(uint64_t *pml)
// {
//     pmm_free(pml);
// }

void vswitch(uint64_t *pml)
{
    pml4 = pml;
}
