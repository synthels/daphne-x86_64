#include <uefi.h>

#define EFI_CONVENTIAL_MEMORY 7
#define EFI_RESERVED_MEMORY   0

#define MEMORY_AVAILABLE 1
#define MEMORY_RESERVED  2

/*** ELF64 defines and structs ***/
#define ELFMAG      "\177ELF"
#define SELFMAG     4
#define EI_CLASS    4       /* File class byte index */
#define ELFCLASS64  2       /* 64-bit objects */
#define EI_DATA     5       /* Data encoding byte index */
#define ELFDATA2LSB 1       /* 2's complement, little endian */
#define ET_EXEC     2       /* Executable file */
#define PT_LOAD     1       /* Loadable program segment */
#ifdef __x86_64__
#define EM_MACH     62      /* AMD x86-64 architecture */
#endif
#ifdef __aarch64__
#define EM_MACH     183     /* ARM aarch64 architecture */
#endif

typedef struct {
	uint8_t  e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} Elf64_Ehdr;

typedef struct {
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
} Elf64_Phdr;

typedef struct {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} memory_map_t;

typedef struct {
	efi_memory_descriptor_t *map;
	uintn_t size;
	uintn_t desc_size;
} efi_mmap_t;

void err(const char *msg)
{
	printf("boot error: %s\n", msg);
	for(;;);
}

void get_mmap(efi_mmap_t *mmap)
{
	efi_status_t status;
	efi_memory_descriptor_t *memory_map = NULL, *mement;
	uintn_t memory_map_size=0, map_key=0, desc_size=0, i;

	status = BS->GetMemoryMap(&memory_map_size, NULL, &map_key, &desc_size, NULL);
	if (status != EFI_BUFFER_TOO_SMALL || !memory_map_size) {
		err("unable to get memory map!");
	}

	memory_map_size += 4 * desc_size;
	memory_map = (efi_memory_descriptor_t*) malloc(memory_map_size);
	if (!memory_map) {
		err("malloc failed!");
	}

	status = BS->GetMemoryMap(&memory_map_size, memory_map, &map_key, &desc_size, NULL);
	if (EFI_ERROR(status)) {
		err("internal UEFI error");
	}

	efi_mmap_t efi_mmap;
	efi_mmap.map = memory_map;
	efi_mmap.size = memory_map_size;
	efi_mmap.desc_size = desc_size;
	*mmap = efi_mmap;
}

void load_kernel(void)
{
	FILE *f;
	char *buff;
	long int size;
	Elf64_Ehdr *elf;
	Elf64_Phdr *phdr;
	uintptr_t entry;
	int i, j;

	if ((f = fopen("kernel.bin", "r"))) {
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		buff = malloc(size + 1);
		if (!buff) {
			err("malloc failed!");
		}
		fread(buff, size, 1, f);
		fclose(f);
	} else {
		err("unable to boot to kernel!");
	}

	/* is it a valid ELF executable for this architecture? */
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
		err("bad kernel binary");
	}

	free(buff);

	/* Execute kernel */
	exit_bs();
	i = (*((int(* __attribute__((sysv_abi)))(void))(entry)))();
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	/* Pass memory map to kernel */
	efi_mmap_t efi_mmap;
	get_mmap(&efi_mmap);
	efi_memory_descriptor_t *map = efi_mmap.map;
	memory_map_t *mmap = malloc(sizeof(memory_map_t) * 255);
	for(efi_memory_descriptor_t *mement = map; (uint8_t *) mement < (uint8_t *) map + efi_mmap.size;
		mement = NextMemoryDescriptor(mement, efi_mmap.desc_size)) {
			mmap->size = 20; /* Default grub size? */
			mmap->base_addr = mement->PhysicalStart;
			/* Convert UEFI type to something the kernel can understand */
			if (mement->Type == EFI_CONVENTIAL_MEMORY) mmap->type = MEMORY_AVAILABLE;
			if (mement->Type == EFI_RESERVED_MEMORY) mmap->type = MEMORY_RESERVED;
	}

	/* TODO: Pass memory map to kernel (move to rax) */
	load_kernel();
	return 0;
}
