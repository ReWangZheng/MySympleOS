	times 512 db 0
	mov ax,0x600
	mov bx,0x700
	mov cx,0x00
	mov dx,0x184f
	int 10h
	times 512 db 0xff
	
