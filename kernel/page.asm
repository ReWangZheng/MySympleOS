global open_pagemode
global page_catalog
global page_table_4MB
page_catalog dd 0x20000
page_table_4MB dd 0x21000
open_pagemode:
    ;参数1：页目标表信息
    push ebp
    mov ebp,esp
    push eax
    mov eax,[ebp+8]
    mov cr3,eax
    mov eax,cr0
    or eax,0x80000000
    mov cr0,eax
    pop eax
    pop ebp
    ret