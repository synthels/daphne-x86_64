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
 * tulip - a simple EFI bootloader
 */

#include <uefi.h>
#include "elf.h"

typedef struct {
	efi_memory_descriptor_t *map;
	uintn_t size;
	uintn_t desc_size;
} efi_mmap_t;

typedef struct {
	uintn_t fb_base;
	uintn_t pps;
	uintn_t width;
	uintn_t height;
} efi_gop_info_t;

typedef struct {
	efi_mmap_t *mmap;
	efi_gop_info_t *gop;
} efi_info_t;

void set_video_mode(efi_gop_info_t *gop_info)
{
	efi_status_t status;
	efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	efi_gop_t *gop = NULL;
	efi_gop_mode_info_t *info = NULL;
	uintn_t isiz = sizeof(efi_gop_mode_info_t), currentMode;

	status = BS->LocateProtocol(&gopGuid, NULL, (void **) &gop);
	if (!EFI_ERROR(status) && gop) {
		/* jump to last mode & set it */
		uintn_t mode = gop->Mode->MaxMode - 1;
		status = gop->SetMode(gop, mode);
		/* get mode info */
		status = gop->QueryMode(gop, gop->Mode ? gop->Mode->Mode : 0, &isiz, &info);

		if (EFI_ERROR(status)) {
			err("unable to set video mode");
		}

		gop_info->width = gop->Mode->Information->HorizontalResolution;
		gop_info->height = gop->Mode->Information->VerticalResolution;
		gop_info->pps = gop->Mode->Information->PixelsPerScanLine;
		gop_info->fb_base = gop->Mode->FrameBufferBase;
	} else {
		err("unable to get GOP");
	}
}

void get_mmap(efi_mmap_t *mmap)
{
	efi_status_t status;
	efi_memory_descriptor_t *memory_map = NULL;
	uintn_t memory_map_size=0, map_key=0, desc_size=0, i;

	status = BS->GetMemoryMap(&memory_map_size, NULL, &map_key, &desc_size, NULL);
	if (status != EFI_BUFFER_TOO_SMALL || !memory_map_size) err("UEFI error");

	memory_map_size += 4 * desc_size;
	memory_map = (efi_memory_descriptor_t*) malloc(memory_map_size);
	if (!memory_map) {
		err("malloc failure");
	}

	status = BS->GetMemoryMap(&memory_map_size, memory_map, &map_key, &desc_size, NULL);
	if (EFI_ERROR(status)) {
		err("couldn't get mmap");
	}

	mmap->map = memory_map;
	mmap->size = memory_map_size;
	mmap->desc_size = desc_size;
}

void load_kernel(efi_info_t *info)
{
	FILE *f;
	char *buff;
	size_t size;
	uintptr_t entry;

	if ((f = fopen("\\EFI\\BOOT\\kernel.bin", "r"))) {
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		buff = malloc(size + 1);
		fread(buff, size, 1, f);
		fclose(f);
	} else {
		err("kernel image not found!");
	}

	/* Load kernel */
	entry = elf_load(buff);

	free(buff);

	/* Save uefi info in rax */
	asm volatile (
		"mov %0, %%rdi\n"
		: : "r"(&info)
	);

	/* Execute kernel */
	exit_bs();
	int ret = (*((int(* __attribute__((sysv_abi)))(void))(entry)))();
	for(;;);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	/* Get memory map */
	efi_mmap_t efi_mmap;
	efi_gop_info_t gop_info;
	efi_memory_descriptor_t *mement;
	get_mmap(&efi_mmap);

	set_video_mode(&gop_info); /* set highest video mode */

	efi_info_t info;
	info.gop = &gop_info;
	info.mmap =  &efi_mmap;
	load_kernel(&info); /* load kernel */
}
