extern CInterrupt_0
extern exception_handle

global Interrupt_0
global DE_ERR
global TS_ERR
global NP_ERR
global SS_ERR
global GP_ERR
global PF_ERR
Interrupt_0:
    call CInterrupt_0
    mov al,20h
    mov dx,20h
    out dx,al
    iret
;除数为0  0x00
DE_ERR:
    push 1
    jmp exception
;无效的TSS 0x10
TS_ERR: 
    push 2
    jmp exception
;段不存在 0x11
NP_ERR: 
    push 3
    jmp exception
;堆栈段错误 0x12
SS_ERR:
    push 4
    jmp exception
;常规保护错误 0x13
GP_ERR: 
    push 5
    jmp exception
;页错误 0x14
PF_ERR:
    push 6
    jmp exception
exception:
    call exception_handle
    add esp,4*2;0x00000003577d
    iret
