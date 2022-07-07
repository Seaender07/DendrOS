[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

global _start

_start:
	mov [bdrive], dl
	
	mov bp, 0x9000
	mov sp, bp

	call load_kernel

	call pm32_switch

	jmp $


%include "boot/bdrive.asm"
%include "boot/bprint.asm"
%include "boot32/bgdt.asm"
%include "boot32/bswitch.asm"
%include "boot32/bprintpm.asm"



[bits 16]

load_kernel:
	mov bx, KERNEL_OFFSET
	mov dh, 15
	mov dl, [bdrive]
	call disk_load

	ret


[bits 32]


BEGIN_PM32:
	mov ebx, bootmsg
	call bprintpm

	call KERNEL_OFFSET		;; CALL THE LINKABLE ASM
	

	jmp $



bootmsg:
	db "Booting BasicOS...", 0x0a, 0x0d, "Hello!", 0

pm32msg:
	db "pm32 entered...", 0

bdrive:
	db 0

times 510-($-$$) db 0

dw 0xaa55


;times 0x100 dw 0xdada
;times 0x100 dw 0xface
