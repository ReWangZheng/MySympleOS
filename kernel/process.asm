;用来保存寄存器信息的方法
esp_offset equ 40
global show_str_format 
save_process_state:
    ;参数:Process的首地址
    push eax
    push ebx
    push edi
    pushad ;先将所有状态都保存下来
    pushf
    mov edi,[esp+32+8] ;得到首地址
;下面步骤保存了eax....edi，但三esp位置保存错误
    mov eax,8
    pop dword [edi+eax]

.save:
    add eax,4
    pop dword [edi+eax]
    cmp eax,40
    jnz .save 
;下面保存esp
    mov eax,24
    mov ebx,esp
    add ebx,24
    mov [edi+eax],esp
    pop edi
    pop ebx
    pop eax
    ret