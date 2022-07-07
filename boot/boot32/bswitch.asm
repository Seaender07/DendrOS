;	
;
;
;
;


pm32_switch:
	xor ax, ax
	mov ds, ax
	
	cli
	lgdt [gdt_descr]

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:init_pm32


[bits 32]


init_pm32:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call BEGIN_PM32
