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
