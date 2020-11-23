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
int_times dd 0 
Interrupt_0:
    pushad   ;addr:0x35e80
    ;下面代码为进程切换代码
    mov eax,[int_times]
    cmp eax,0
    jnz end
    ;标记一下中断数量
    inc eax
    mov dword [int_times],eax
    sti
    ;通知处理器可以接受中断了s
    mov al,20h
    mov dx,20h
    out dx,al
    ;处理时间处理函数
    call CInterrupt_0
    ;得到当前进程
    call getCurrentP
    cmp eax,0
    jz noprocess
    add dword [esp+12],12
    mov ecx,11
    xor edx,edx
    xor ebx,ebx
    ;开始保存现场
.save:
    mov edx,[esp+ebx]
    mov [eax+ebx],edx
    add ebx,4
    loop .save
noprocess:
    call fetch
    cmp eax,0
    jz end
    ;下面是要重新加载页目录
    mov ebx,[eax+64]
    mov cr3,ebx
    ;进入程序栈
    add esp,44
    mov esp,[eax+12]
    add eax,40
    mov ecx,11
    ;恢复堆栈
restory:
    push dword [eax]
    sub eax,4
    loop restory
end:
    popad
    ;设置标记为0
    mov dword [int_times],0
    iretd
;除数为0  0x00
DE_ERR:
    pushad
    push 1
    jmp exception
;无效的TSS 0x10
TS_ERR: 
    pushad
    push 2
    jmp exception
;段不存在 0x11
NP_ERR: 
    pushad
    push 3
    jmp exception
;堆栈段错误 0x12
SS_ERR:
    pushad
    push 4
    jmp exception
;常规保护错误 0x13
GP_ERR: 
    pushad
    push 5
    jmp exception
;页错误 0x14
PF_ERR:
    pushad
    push 6
    jmp exception
exception:
    call exception_handle
    add esp,4
    popad
    add esp,4;0x00000003577d
    iretd


    