extern cstart
extern gdt_ptr
global _start
str_add db 'wo hao ai ni ya!!',0
_start:
    mov ax,100
    mov bx,100
    mov cx,100
    call cstart
    lgdt [gdt_ptr]
    jmp $
