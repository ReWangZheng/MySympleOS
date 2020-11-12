;导出函数
global memcpy
extern test
global _start
;void* memcpy(void * des,void* source,int sizes); 函数原型
memcpy:
    push ebp
    mov ebp,esp
    push eax
    push esi
    push ecx
    push edi
    mov esi,[ebp+8] ;等待复制的目的地址
    mov edi,[ebp+12] ;源地址
    mov ecx,[ebp+16] ;需要复制的大小
.copy:
    mov al,[ds:esi]
    mov [es:edi],al
    inc edi
    inc esi
    loop .copy
    pop edi
    pop ecx
    pop esi
    pop eax
    pop ebp
    ret
[section .data]	; 数据在此
num1st		dd	3
num2nd		dd	4
[section text]
_start:
    push eax
    push ebx
    call test
    mov	ebx, 0
	mov	eax, 1		; sys_exit
	int	0x80		; 系统调用