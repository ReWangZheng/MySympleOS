;导出函数 void GetGDT(GDT * gdt);
global GetGDT
GetGDT:
    ;参数1：要存储GDT的内存地址
    push ebp
    mov ebp,esp
    push eax
    mov eax,[ebp+8]
    sgdt [eax]
    pop eax
    pop ebp
    ret
global LoadGDT
;导出函数:LoadGdt(u32 *address)
LoadGDT:
    ;参数1：gdt的地址
    push ebp
    mov ebp,esp
    push eax
    mov eax,[ebp+8]
    lgdt [eax]
    pop eax
    pop ebp
    ret
global LoadIDT
;导出函数:LoadIDT(u32 *address)
LoadIDT:
    ;参数1：IDT的地址
    push ebp
    mov ebp,esp
    push eax
    mov eax,[ebp+8]
    lidt [eax]
    pop eax
    pop ebp
    ret
global LoadLDT
;导出函数:LoadLDT(u32 *add)
LoadLDT:
    ;参数1：IDT的地址
    push ebp
    mov ebp,esp
    push eax
    mov eax,[ebp+8]
    lldt ax
    pop eax
    pop ebp
    ret
global getEAX
global getEBX
global getECX
global getEDX
global getESP
global getSS
global getDS
global getCS
global getEFLAGE 
getEAX:
    ret
getEBX:
    mov eax,ebx
    ret
getECX:
    mov eax,ecx
    ret
getEDX:
    mov eax,edx
    ret
getESP:
    mov eax,esp
    ret
getSS:
    mov eax,ss
    ret
getDS:
    mov eax,ds
    ret
getCS:
    mov eax,cs
    ret
getEFLAGE:
    push ebp
    mov ebp,esp
    pushf
    mov eax,[esp]
    sti
    popf
    pop ebp
    ret