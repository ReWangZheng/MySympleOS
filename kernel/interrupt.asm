extern CInterrupt_0
extern exception_handle
extern fetch
extern timer
extern getCurrentP
global Interrupt_0
global DE_ERR
global TS_ERR
global NP_ERR
global SS_ERR
global GP_ERR
global PF_ERR
extern kernel_esp
extern process_esp
extern debug
Interrupt_0:
    call CInterrupt_0
    ;下面代码为进程切换代码
    pushad   ;addr:0x35e80
    call getCurrentP
    cmp eax,0
    jz noprocess
    add dword [esp+12],12
    ;得到当前运行的进程
    mov ecx,11
    xor edx,edx
    xor ebx,ebx
.save:
    mov edx,[esp+ebx]
    mov [eax+ebx],edx
    add ebx,4
    loop .save
    ;sub dword [eax+12],12
noprocess:
    call fetch
    cmp eax,0
    jz end
    add esp,44
    mov esp,[eax+12];进入程序栈
    add eax,40
    mov ecx,11
restory:
    push dword [eax]
    sub eax,4
    loop restory
end:
    mov al,20h
    mov dx,20h
    out dx,al
    popad
    iretd

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
    iretd


    