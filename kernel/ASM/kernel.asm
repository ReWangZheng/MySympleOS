extern SYSTERM_INIT
extern initDestop
global _start
_start:
    mov ax,100  ;0x35012
    mov bx,100
    mov cx,100
    call SYSTERM_INIT
    sti
    jmp $

