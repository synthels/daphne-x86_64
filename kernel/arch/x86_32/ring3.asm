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
	mov gs, ax

	push 0x23
	push esp
	pushfd
	push 0x1b
	lea eax, [user_start]
	push eax
	iretd

user_start:
	add esp, 4
	ret

tss_flush:
	mov ax, 0x2b
	ltr ax
	ret
