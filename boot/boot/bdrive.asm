;
;
;
;
;


disk_load:				; Data to work with stored in 'bx' (placement of read data), 'dh' (no. of sectors to read) and 'dl' (disk drive no.)
	push dx			; Push disk no. ('dl') and the no. of sectors to read ('dh')
	
	mov ah, 0x02		; Set to BIOS read sector funcion

	mov al, dh		; Store in 'al' the no. of the sectors to read
	mov ch, 0x00		; Cylinder no. (0-based)
	mov dh, 0x00		; Diskette side no. (0-based)
	mov cl, 0x02		; Start reading from sector (n*0x200B) (1-based)

	int 0x13
	pop dx
	push bx			; Push read result

	jc rerr			; Jump to error handling if the carry segment has a positive value

	push ax			; Push the no. of actually read sectors ('al')
	cmp dh, al
	jne rerr		; Jump to error handling if the no. of expected read sectors is different from the actually read one

	jmp endpr		; If no errors are reported, jump to end.
	
	rerr:
	mov bx, errmsg
	call prints		; Show the error message
	
	endpr:
	mov bx, endmsg
	call prints		; Show the final message

	pop ax
	pop bx
	mov dx, 0
	mov dl, al		; Pass as argument the no. of actually read sectors
	call print_hex		; Print in hex format the no. of read sectors

	ret


errmsg:
	db "There was an error reading the disk.", 0x0a, 0x0d, 0

endmsg:
	db "Sectors read: ", 0

