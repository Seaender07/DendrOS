;
;
; BasicOS's bootloader's print functions
;
;


hextoascii:			; Data to work with stored in 'cl'
	cmp BYTE cl, 0x0a
	jl fdec				; Jump to conversion of decimals if 'cl' is less than 0x0a (10)

	; Conversion of letters
	add cl, 0x37			; Fill the gap between the hex number and the ascii char
	jmp phfw			; Jump to end

	; Conversion of decimals
	fdec:
	add cl, 0x30			; // (but it's a char representing a decimal digit)

	phfw:
	ret


printc:				; Data to work with stored in 'al'
	mov ah, 0x0e
	int 0x10
	ret


prints:				; Data to work with stored in 'bx'
	psstart:
	cmp BYTE [bx], 0
	je psend		; If the character is '0x00' ('\0', null), jump to end.

	mov al, [bx]
	call printc		; Print single char stored in 'al'
	
	inc bx			; Make 'ebx' point to the next char in string

	jmp psstart		; Repeat the process (end check included)

	psend:
	ret

printnl:
	mov bx, nlcr
	call prints
	ret


print_hex:			; Data to work with stored in 'dx'
	mov bx, shex			; Point 'ebx' to the final string's begin address
	add bx, 2			; Ignore the '0x' hex directive

	; First digit
	mov cl, dh			; Pick the first byte in 'dx'
	shr cl, 4			; Erase the last four digits
	call hextoascii			; Convert 'cl' to ascii chars
	mov [bx], cl			; Move the ascii char stored in 'cl' to the current address in the final string

	; Second digit
	inc bx				; Make 'ebx' point to the next char in the final string
	mov cl, dh			; //
	and cl, 0x0f			; Erase the first four digits
	call hextoascii			; //
	mov [bx], cl			; //

	; Third digit
	inc bx				; //
	mov cl, dl			; Pick the second byte in 'dx'
	shr cl, 4			; Erase the last four digits
	call hextoascii			; //
	mov [bx], cl			; //

	; Fourth digit
	inc bx				; //
	mov cl, dl			; //
	and cl, 0x0f			; Erase the first four digits
	call hextoascii			; //
	mov [bx], cl			; //

	mov bx, shex			; Reset 'ebx' to the beginning of the final string
	call prints			; Print the final string

	ret


shex:
	db "0x0000", 0

nlcr:
	db 0x0a, 0x0d, 0
