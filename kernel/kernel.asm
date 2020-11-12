extern test
global _start
sss dw 0xffff
    times 50 db 0xff
_start:

    mov ax,100
    mov bx,100
    mov cx,100
    call test
    