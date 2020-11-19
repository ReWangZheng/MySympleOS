;用来保存寄存器信息的方法
esp_offset equ 40
global save_process_state 
save_process_state:
    ;参数:Process的首地址
    
    pushad ;先将所有状态都保存下来
    pushf
    mov edi,[esp+32+8] ;得到首地址
    mov eax,8
;下面步骤保存了eax....edi，但三esp位置保存错误
    popf [edi+eax]
.save:
    add eax,4
    pop [edi+eax]
    cmp eax,40
    jnz .save 
;下面保存esp
    mov eax,24
    add esp,36
    mov [edi+eax],esp+12
    ret