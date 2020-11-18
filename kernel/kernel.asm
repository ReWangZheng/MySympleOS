extern cstart
extern clean_screen
extern gdt_ptr ;GDT
extern idt_ptr ;IDT
extern Init_8259
extern init_pagemode
extern clear
global _start
str_add db 'wo hao ai ni ya!!',0
_start:
    mov ax,100  ;0x35012
    mov bx,100
    mov cx,100
    call cstart
    lgdt [gdt_ptr]
    lidt [idt_ptr]
    sti
    call init_pagemode
    mov ax,[0x0f025412]
    jmp $
    ;0x3577a