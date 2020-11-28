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
global keybordInt
extern kernel_esp
extern process_esp
extern debug
extern gdt_ptr
extern keybordHandle
int_times dd 0
keybordInt:
    pushad
    call keybordHandle
    ;通知处理器可以接受中断了
    mov al,20h
    mov dx,20h
    out dx,al
    popad
    iretd
Interrupt_0:
    pushad   ;addr:0x35e80
    ;下面代码为进程切换代码
    mov eax,[int_times]
    cmp eax,0
    jnz end
    ;标记一下中断数量
    inc eax
    mov dword [int_times],eax
    ;开中断
    sti
    ;通知处理器可以接受中断了
    mov al,20h
    mov dx,20h
    out dx,al
    ;处理时间处理函数
    call CInterrupt_0
    ;得到当前进程
    call getCurrentP
    cmp eax,0
    jz noprocess
    ;将esp恢复一下
    add dword [esp+12],12
    ;下面保护段寄存器
    push gs;
    push fs;
    push ss;
    push es;
    push ds;
    ;..................
    ;将段寄存器的值存到Process结构体中
    mov ecx,5
    xor edx,edx
    xor ebx,ebx
    mov ebx,68
.save_seg:
    pop edx
    and edx,0x0000ffff
    mov dword [eax+ebx],edx
    add ebx,4
    loop .save_seg
    ;下面保存popad中的寄存器值
    mov ecx,11
    xor edx,edx
    xor ebx,ebx
.save:
    mov edx,[esp+ebx]
    mov [eax+ebx],edx
    add ebx,4
    loop .save
noprocess:
    ;问题1：为什么edx的值会发生变化？？
    push eax
    call fetch
    pop edx
    cmp eax,0
    jz end
    ;判断两个进程是不是相同的，相同则没必要重新加载页目录和IDT、TSS了
    cmp eax,edx
    jz restory_start
    ;下面是要重新加载页目录
    mov ebx,[eax+64]
    mov cr3,ebx ;0008:0000000000035c6a
    ;下面开始加载LDT
    mov ebx,[eax+56] ;得到ldt的选择子
    lldt bx
    mov ebx,[eax+48] ;得到tss的选择子
restory_start:
    ;将之前的堆栈清除掉
    add esp,44
    ;进入程序栈
    mov esp,[eax+12]
    ;指针指向EFLAGE
    add eax,40
    mov ecx,11
    ;恢复堆栈
restory:
    push dword [eax]
    sub eax,4
    loop restory
    ;将指针指向ds
    add eax,72
    mov ecx,5
restory_seg:
    push dword [eax]
    add eax,4
    loop restory_seg
end:
    pop gs
    pop fs
    pop ss
    pop es
    pop ds
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
global close_int
global open_int
close_int:
    cli
    ret
open_int:
    sti
    ret

    