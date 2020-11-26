;--------------------------------------
global getcursor
;u16 getcursor()
getcursor:
    ;返回:光标位置，放在ax中
    push dx
    push bx
    mov dx,0x3d4
    mov al,0x0e
    out dx,al
    mov dx,0x3d5
    in  al,dx
    mov bh,al
    mov dx,0x3d4
    mov al,0x0f
    out dx,al
    mov dx,0x3d5
    in al,dx
    mov bl,al
    mov ax,bx
    pop bx
    pop dx
    ret 
;-------------------------------------------
global SetCursor
;void SetCursor(int x,int y)
SetCursor:
        ;参数1:列
        ;参数2:行
        push ebp
        mov ebp,esp
        push edx
        push eax
        mov eax,[ebp+12]

        mov bx,80
        mul bx
        add eax,[ebp+8]
        mov ebx,eax

        mov dx,0x3d4
        mov al,0x0f
        out dx,al
        mov dx,0x3d5
        mov al,bl
        out dx,al

        shr ebx,8

        mov dx,0x3d4
        mov al,0x0e
        out dx,al
        mov dx,0x3d5

        mov al,bl
        out dx,al
        pop eax
        pop edx
        pop ebp
        ret

;--------------------------;--------------------------
global out_byte
;void out_byte(u16 port,u8 value);
out_byte:
    ;参数1：端口号
    ;参数2：值
    push ebp
    mov ebp,esp
    push edx
    mov edx,[ebp+8]
    mov eax,[ebp+12]
    out dx,al
    nop
    nop
    nop
    nop
    pop edx
    pop ebp
    ret
;--------------------------------------------
global in_byte
in_byte:
    ;参数1：端口号
    push ebp
    mov ebp,esp
    push edx
    mov edx,[ebp+8]
    xor eax,eax
    in al,dx
    nop
    nop
    pop edx
    pop ebp
    ret
;---------------------
global clear
clear:
	;清屏
    push eax
    push ebx
    push edi
    mov ebx,0
    mov edi,0xb8000
    mov al,' '
    mov ah,07
.@1:
    mov [edi+ebx],ax
    cmp ebx,4000
    add ebx,2
    jnz .@1
    mov eax,0x
    pop edi
    pop eax
    pop ebx
    ret
