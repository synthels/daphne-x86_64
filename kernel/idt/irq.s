; Assembly IRQ dummy handlers

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

global load_idt

extern irq0_handler
extern irq1_handler
extern irq2_handler
extern irq3_handler
extern irq4_handler
extern irq5_handler
extern irq6_handler
extern irq7_handler
extern irq8_handler
extern irq9_handler
extern irq10_handler
extern irq11_handler
extern irq12_handler
extern irq13_handler
extern irq14_handler
extern irq15_handler

irq0:
	pushad
	cld
	call irq0_handler
	popad
	iret

irq1:
	pushad
	cld
	call irq1_handler
	popad
	iret

irq2:
	pushad
	cld
	call irq2_handler
	popad
	iret

irq3:
	pushad
	cld
	call irq3_handler
	popad
	iret

irq4:
	pushad
	cld
	call irq4_handler
	popad
	iret

irq5:
	pushad
	cld
	call irq5_handler
	popad
	iret

irq6:
	pushad
	cld
	call irq6_handler
	popad
	iret

irq7:
	pushad
	cld
	call irq7_handler
	popad
	iret

irq8:
	pushad
	cld
	call irq8_handler
	popad
	iret

irq9:
	pushad
	cld
	call irq9_handler
	popad
	iret

irq10:
	pushad
	cld
	call irq10_handler
	popad
	iret

irq11:
	pushad
	cld
	call irq11_handler
	popad
	iret

irq12:
	pushad
	cld
	call irq12_handler
	popad
	iret

irq13:
	pushad
	cld
	call irq13_handler
	popad
	iret

irq14:
	pushad
	cld
	call irq14_handler
	popad
	iret

irq15:
	pushad
	cld
	call irq15_handler
	popad
	iret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret
