extern cstart
extern clean_screen
extern gdt_ptr ;GDT
extern idt_ptr ;IDT
extern Init_8259
extern init_pagemode
extern clear
global kernel_esp
global _start
global process_esp

kernel_esp dd 0x00000000
_start:
    mov ax,100  ;0x35012
    mov bx,100
    mov cx,100
    mov dword [kernel_esp],esp
    call cstart
    ;call processhandle
    sti
    jmp $
    ;0x3577a