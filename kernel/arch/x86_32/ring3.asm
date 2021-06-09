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
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; Ring 3
;;

[bits 32]

extern user_entry

global enter_usermode
global tss_flush

enter_usermode:
	mov ax, 0x23
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax ; no need for ss

	mov eax, esp ; setup stack frame
	push 0x23 ; ds
	push eax ; esp
	pushf ; eflags
	push 0x23 ; cs
	push user_entry ; return
	iret

tss_flush:
	mov ax, 0x2b
	ltr ax
	ret
