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

global LoadGdt
;导出函数:LoadGdt(u32 *address)
LoadGDT:
    ;参数1：gdt的地址
    push ebp
    mov ebp,esp
    lgdt [ebp+8]
    pop ebp
    ret
global LoadIDT
;导出函数:LoadIDTu32 *address)
LoadIDT:
    ;参数1：IDT的地址
    push ebp
    mov ebp,esp
    lidt [ebp+8]
    pop ebp
    ret