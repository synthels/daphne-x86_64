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

void map_kernel()
{
    /* Map memory map */
    mmap_entry_t *mmap = get_memsp()->mmap;
    for (size_t i = 0; i < get_memsp()->size; i++) {
        mmap_entry_t *entry = &mmap[i];
        size_t base = entry->base - (entry->base % PAGE_SIZE);
        size_t length = MMU_CEIL(entry->length, PAGE_SIZE) * PAGE_SIZE;
        if (entry->base % PAGE_SIZE)
            length += PAGE_SIZE;

        for (size_t j = 0; j * PAGE_SIZE < length; j++) {
            size_t addr = base + j * PAGE_SIZE;
            map_page(MEM_PHYS_OFFSET + addr, addr, VMM_DEFAULT_PERMISSIONS);
        }
    }

    /* Map 0x00000000 -> 0xffffffff80000000 as per the spec */
    for (uint64_t addr = 0x0; addr < 0x80000000; addr += PAGE_SIZE) {
        map_page(addr + MEM_KERN_OFFSET, addr, VMM_DEFAULT_PERMISSIONS);
    }

    /* Map kernel heap */
    for (uint64_t i = 0; i < HEAP_PAGES; i++) {
        map_page(KERNEL_HEAP_LOW + (i * PAGE_SIZE), (uint64_t) pmm_alloc(PAGE_SIZE), VMM_DEFAULT_PERMISSIONS);
    }

    /* Load pml4 */
    asm volatile ("movq %0, %%cr3" : : "r"((uint64_t) pml4));
}

void vmm_init(void)
{
    pml4 = pmm_alloc_page();
    memset(pml4, 0, PAGE_SIZE);
    /* Recursive mapping */
    pml4[511] = (uint64_t) pml4;
    map_kernel();
}

uint64_t *next_level_or_alloc(uint64_t *current_level, size_t entry)
{
    if (!(current_level[entry] & 1)) {
        current_level[entry] = (uint64_t) pmm_alloc_page();
        memset((void *) current_level[entry], 0, PAGE_SIZE);
        current_level[entry] |= 3;
    }
    return (uint64_t *)((current_level[entry] & ~(0x1ff)));
}

void map_page(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags)
{
    /* Calculate every index */
    size_t pml4_entry = (virt_addr >> 39) & 0x1ff; 
    size_t pml3_entry = (virt_addr >> 30) & 0x1ff; 
    size_t pml2_entry = (virt_addr >> 21) & 0x1ff; 
    size_t pml1_entry = (virt_addr >> 12) & 0x1ff; 

    /* Traverse levels */
    uint64_t *pml3 = next_level_or_alloc(pml4, pml4_entry);
    uint64_t *pml2 = next_level_or_alloc(pml3, pml3_entry);
    uint64_t *pml1 = next_level_or_alloc(pml2, pml2_entry);

    /* Create mapping */
    pml1[pml1_entry] = (uint64_t) (phys_addr | flags);
}
