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
 * x86_64 virtual memory manager
 */

#include "vmm.h"

static uint64_t *pml4;

extern void arch_reload_page_table(uintptr_t);

uint64_t *vmm_get_pml4(void)
{
    return pml4;
}

uint64_t *next_level_or_alloc(uint64_t *current_level, size_t entry, uint64_t flags)
{
    if (!(current_level[entry] & 1)) {
        current_level[entry] = (uint64_t) pmm_alloc_page();
        memset((void *) current_level[entry], 0, PAGE_SIZE);
        current_level[entry] |= flags;
    }
    return (uint64_t *)((current_level[entry] & ~(0x1ff)));
}

size_t get_pml_index(uint64_t addr)
{
    return (addr >> 12) & 0x1ff;
}

/**
 * @brief Copy a page from src to dst
 */
void copy_page(uint64_t *src, uint64_t *dst, uint64_t addr, uint64_t flags)
{
    size_t idx = get_pml_index(addr);
    map_page(dst, addr, src[idx], flags);
}

/**
 * @brief Map all memory needed by the processor
 *
 * Maps the regions 0x00000000->0x100000000, 0x100000000->0xffffffff80000000
 */
void map_spec(uint64_t *pml)
{
    for (uintptr_t i = 0; i < 4 * GB; i += PAGE_SIZE) {
        map_page(pml, MEM_PHYS_OFFSET + i, i, KERNEL_PAGE_ACCESS);
        map_page(pml, i, i, KERNEL_PAGE_ACCESS);
    }

    for (uint64_t addr = 0; addr < 0x80000000; addr += PAGE_SIZE) {
        map_page(pml, addr + MEM_KERN_OFFSET, addr, KERNEL_PAGE_ACCESS);
    }
}

/**
 * @brief Map kernel into PML
 *
 * Maps the kernel heap
 */
void map_kernel(uint64_t *pml, bool load_pml)
{
    map_spec(pml);
    if (load_pml) {
        /* Map kernel heap */
        for (uint64_t i = 0; i < HEAP_PAGES; i++) {
            map_page(pml, KERNEL_HEAP_LOW + (i * PAGE_SIZE), (uint64_t) pmm_alloc_page(), KERNEL_PAGE_ACCESS);
        }
        arch_reload_page_table((uintptr_t) pml);
    } else {
        /* Copy heap over */
        for (uint64_t i = 0; i < HEAP_PAGES; i++) {
            copy_page(pml4, pml, KERNEL_HEAP_LOW + (i * PAGE_SIZE), KERNEL_PAGE_ACCESS);
        }
    }
}

/**
 * @brief Initialize VMM
 *
 * Creates a page table and maps the kernel into it
 */
void vmm_init(void)
{
    pml4 = pmm_alloc_page();
    memset(pml4, 0, PAGE_SIZE);
    /* Recursive mapping */
    pml4[511] = (uint64_t) pml4;
    map_kernel(pml4, true);
}

/**
 * @brief Initialize a PML
 *
 * Maps the kernel into a PML
 */
void vmm_init_pml(uint64_t *pml)
{
    memset(pml, 0, PAGE_SIZE);
    pml[511] = (uint64_t) pml;
    map_kernel(pml, false);
}

/**
 * @brief Map a page into the kernel's PML
 *
 * Maps the kernel into a PML
 */
void pml4_map_page(uint64_t virt_addr, uint64_t phys_addr, uint64_t flags)
{
    map_page(pml4, virt_addr, phys_addr, flags);
}

/**
 * @brief Map a page into a PML
 *
 * Creates a mapping for phys_addr to virt_addr
 */
void map_page(uint64_t *pml, uint64_t virt_addr, uint64_t phys_addr, uint64_t flags)
{
    /* Calculate every index */
    size_t pml4_entry = (virt_addr >> 39) & 0x1ff;
    size_t pml3_entry = (virt_addr >> 30) & 0x1ff;
    size_t pml2_entry = (virt_addr >> 21) & 0x1ff;
    size_t pml1_entry = (virt_addr >> 12) & 0x1ff;

    /* Traverse levels */
    uint64_t *pml3 = next_level_or_alloc(pml, pml4_entry, flags);
    uint64_t *pml2 = next_level_or_alloc(pml3, pml3_entry, flags);
    uint64_t *pml1 = next_level_or_alloc(pml2, pml2_entry, flags);

    /* Create mapping */
    pml1[pml1_entry] = (uint64_t) (phys_addr | (flags & 0xFFF));
}

/**
 * @brief Allocate a new PML
 *
 * Creates a new PML for a process
 */
uint64_t *vmalloc(void)
{
    uint64_t *pml = pmm_alloc_page();
    vmm_init_pml(pml);

    user_virt_map(pml, PROC_STACK_LOW, PROC_STACK_SIZE);
    user_virt_map(pml, PROC_HEAP_LOW, PROC_HEAP_SIZE);

    return pml;
}

/**
 * @brief Free PML
 *
 * Frees a PML
 */
void vfree(uint64_t *pml)
{
    pmm_free(pml);
}

/**
 * @brief Swap page directory of this core to pml
 */
void swap_page_directory(uint64_t *pml)
{
    /* TODO: Handle this_core->id > 0 */
    if (this_core->is_bsp) {
        arch_reload_page_table((uintptr_t) pml);
    }
}
