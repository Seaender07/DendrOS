[bits 32]
;
;
;
;
;


VIDMEM equ 0xb8000			; Start address of video memory
FWH_BBL equ 0x0f			; White foreground / Black background color code


bprintpm:			; Data to work with stored in 'ebx'
	pusha
	mov edx, VIDMEM
	mov ah, FWH_BBL			; Move in 'ah' the color code

	bppm_loop:
	mov al, [ebx]			; Move in 'al' the current char

	cmp al, 0
	je bppm_done			; Jump to end if '\0' is found

	call bprintepm
	cmp al, 0x20
	jl aprpm	

	mov [edx], ax			; Move the two data to the current memory address
	aprpm:

	add ebx, 1			; Point to next char
	add edx, 2			; Point to next char slot (+1 for color code storage)

	jmp bppm_loop			; Repeat with next char

	bppm_done:
	popa
	ret


bclearsc:
	pusha
	mov edx, VIDMEM

	mov al, ' '
	mov ah, FWH_BBL
	
	xor cx, cx
	
	bcs_row:
	mov ch, 0
	
	cmp cl, 25
	jge bcs_ldone
	
	bcs_col:
	cmp ch, 80
	jge bcs_cdone

	mov [edx], ax
	add edx, 2

	inc ch
	jmp bcs_col

	bcs_cdone:
	inc cl
	jmp bcs_row

	bcs_ldone:
	popa
	ret


bprintepm:
	push ecx
	mov ecx, edx
	sub ecx, (VIDMEM - 2)
	
	cmp al, 0x0d
	je pmcr
	cmp al, 0x0a
	je pmnl

	jmp pmesc_done

	pmcr:
	cmp ecx, 2*80
	jle pmcr_lsub_done
	sub ecx, 2*80
	jmp pmcr
	pmcr_lsub_done:
	sub edx, ecx
	jmp pmesc_done

	pmnl:
	add edx, 2*80
	jmp pmesc_done

	pmesc_done:
	pop ecx
	ret
