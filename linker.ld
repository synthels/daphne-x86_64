/* Entry */
ENTRY(_start)

SECTIONS
{
	/* Begin sections at 1 MiB, since lower memory is used by GRUB */
	. = 1M;

	/* Multiboot header & text section */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Initialized data */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Uninitialized data & stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
