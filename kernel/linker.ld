/* Entry */
ENTRY(_start)

SECTIONS
{
	kstart = .;

	. = 0x01000000;

	/* Then let's place all the other traditional executable sections afterwards. */
	.text : {
		*(.text*)
	}

	.rodata : {
		*(.rodata*)
	}

	.data : {
		*(.data*)
	}

	.bss : {
		*(COMMON)
		*(.bss*)
	}

	kend = .;
}
