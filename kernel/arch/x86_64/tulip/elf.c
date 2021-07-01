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
 * Limited ELF Loader
 */

#include "elf.h"

void err(const char *msg)
{
	printf("boot error: %s\n", msg);
	for(;;);
}

uintptr_t elf_load(char *buf)
{
	char *buff;
	Elf64_Ehdr *elf;
	Elf64_Phdr *phdr;
	uintptr_t entry;
    int i;

	elf = (Elf64_Ehdr *) buff;
	if (!memcmp(elf->e_ident, ELFMAG, SELFMAG) && /* magic match? */
		elf->e_ident[EI_CLASS] == ELFCLASS64 &&   /* 64 bit? */
		elf->e_ident[EI_DATA] == ELFDATA2LSB &&   /* LSB? */
		elf->e_type == ET_EXEC &&                 /* executable object? */
		elf->e_machine == EM_MACH &&              /* architecture match? */
		elf->e_phnum > 0) {                       /* has program headers? */
			/* load segments */
			for (phdr = (Elf64_Phdr *)(buff + elf->e_phoff), i = 0;
				i < elf->e_phnum;
				i++, phdr = (Elf64_Phdr *)((uint8_t *)phdr + elf->e_phentsize)) {
					if (phdr->p_type == PT_LOAD) {
						printf("ELF segment %p %d bytes (bss %d bytes)\n", phdr->p_vaddr, phdr->p_filesz,
							phdr->p_memsz - phdr->p_filesz);
						memcpy((void*)phdr->p_vaddr, buff + phdr->p_offset, phdr->p_filesz);
						memset((void*)(phdr->p_vaddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);
					}
				}
			entry = elf->e_entry;
	} else {
		err("corrupted kernel");
	}

    return entry;
}
