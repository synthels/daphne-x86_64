;;
; Copyright (C) 2020 synthels <synthels.me@gmail.com>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; GDT
;;

[bits 32]

global load_gdt
extern setcs

gdt_start:
	gdt_null: ; null descriptor
		dd 0x0
		dd 0x0

	gdt_code: ; code segment descriptor
		dw 0xffff ; limit (bits 0-15)
		dw 0x0 ; base (bits 0-15)
		db 0x0 ; base (bits 16 -23)
		db 10011010b ; 1st flags, type flags
		db 11001111b ; 2nd flags, Limit (bits 16-19)
		db 0x0 ; base (bits 24 - 31)

	gdt_data: ; data segment descriptor
		dw 0xffff ; limit (bits 0-15)
		dw 0x0 ; base (bits 0-15)
		db 0x0 ; base (bits 16 -23)
		db 10010010b ; 1st flags, type flags
		db 11001111b ; 2nd flags, Limit (bits 16-19)
		db 0x0 ; base (bits 24 - 31)

	gdt_end:
		gdt_descriptor:
			dw gdt_end - gdt_start - 1 ; size of the gdt
			dd gdt_start ; gdt start address

; load gdt
load_gdt:
	lgdt [gdt_descriptor] ; load GDT
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:setcs
