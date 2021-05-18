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
; Paging init
;;

global load_page_dir
global init_paging

load_page_dir:
	push ebp
	mov ebp, esp
	mov eax, [esp + 8]
	mov cr3, eax
	mov esp, ebp
	pop ebp
	ret

init_paging:
	push ebp
	mov ebp, esp
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	mov esp, ebp
	pop ebp
	ret
