;
;
;	GDT
;
;


gdt_start:

gdt_null:			; Set the mandatory null descriptor
	dd 0x0
	dd 0x0

gdt_code:			; Code segment descriptor
	dw 0xffff			; Limit
	dw 0x0				; Base (bits 0-15)
	db 0x0				; Base (bits 16-23)
	db 10011010b			; 1st type flags (present, privilege, desc. type) + type flags (code, conforming, readable, accessed)
	db 11001111b			; 2nd type flags (granularity, 32-bit default, 64-bit seg, AVL) + limit (bits 16-19)
	db 0x0				; Base (bits 24-31)

gdt_data:			; Data segment descriptor
	dw 0xffff			; //
	dw 0x0				; //
	db 0x0				; //
	db 10010010b			; //
	db 11001111b			; //
	db 0x0				; //

gdt_end:

gdt_descr:
	dw gdt_end - gdt_start - 1	; GDT's size
	dd gdt_start			; GDT's start address


CODE_SEG equ gdt_code - gdt_start	; Segment index
DATA_SEG equ gdt_data - gdt_start	; //
