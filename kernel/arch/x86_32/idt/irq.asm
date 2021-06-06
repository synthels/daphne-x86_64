global irq0
global irq1
global _syscall
global generic_irq

global load_idt

extern pit_irq_handler
extern kbd_irq_handler
extern generic_irq_handler
extern syscall_handler

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

irq0:
	pushad
	cld
	call pit_irq_handler
	popad
	iret

irq1:
	pushad
	cld
	call kbd_irq_handler
	popad
	iret

_syscall:
	pushad
	cld
	call syscall_handler
	popad
	iret

generic_irq:
	pushad
	cld
	call generic_irq_handler
	popad
	iret
