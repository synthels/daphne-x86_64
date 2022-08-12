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
 * elf64 loader
 */

#include "elf.h"

bool elf_verify(struct elf_header *header)
{
    return ((
        header->e_ident[0] == ELFMAG0 &&
        header->e_ident[1] == ELFMAG1 &&
        header->e_ident[2] == ELFMAG2 &&
        header->e_ident[3] == ELFMAG3
    )
    && (header->e_ident[EI_CLASS] == ELFCLASS_64));
}

void elf_load(void *elf, struct context *c, struct elf_stat *st)
{
    struct elf_header *header = (struct elf_header *) elf;
    /* Incorrect header, oh well... */
    if (!elf_verify(elf)) {
        st->loaded = false;
        return;
    }

    /* Load all loadable segments */
    void *parent = vmm_get_pml4();
    mmu_switch(c);

    for (uint16_t i = 0; i < header->e_phnum; i++) {
        struct elf_phdr *phdr = ((struct elf_phdr *) (elf + header->e_phoff + i * header->e_phentsize));
        /* Skip if header is empty */
        if (!phdr->p_memsz) {
            continue;
        }

        size_t misalign = phdr->p_vaddr & (PAGE_SIZE - 1);
        uintptr_t addr = phdr->p_vaddr - misalign;
        size_t length = (phdr->p_memsz + misalign + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

        /* Load segment */
        st->entry = header->e_entry;
        c->entry = header->e_entry;
        if (phdr->p_type == PT_LOAD) {
            mmap_current(addr, length);
            memset((void *) (phdr->p_vaddr + phdr->p_filesz), 0, (phdr->p_memsz - phdr->p_filesz));
            memcpy((void *) (phdr->p_vaddr), (void *) (elf + phdr->p_offset), phdr->p_filesz);
        }
    }

    c->regs->rsp = PROC_STACK_LOW;
    /* Switch back to running task */
    mmu_switch(parent);

    st->loaded = true;
}
