;导出函数
global display_str
;void display_str(char *str,int x,int y)
display_str:
	;参数1：串的地址
	;参数2：显示坐标行
    ;参数3：显示的坐标列
	push ebp
	mov ebp,esp
    ;首先计算行开始的字节
    mov ax,0x18
    mov es,ax
    mov eax,[ss:ebp+16]
    mov bx,160
    mul bx
    add dword eax,[ss:ebp+12]
    add dword eax,[ss:ebp+12]
    mov edi,eax
    mov esi,[ss:ebp+8] ;串地址
    mov ah,0x07
.read_str:
    mov al,[esi]
	cmp al,0
    jz .end
    cmp al,0x0a ;如果是换行符
    jz .add_line
    ;下面正常显示字符
    mov [es:edi],ax
    add edi,2
    inc esi
    jmp .read_str
.add_line:
    push eax
    push ebx
    push edx
    mov eax,edi
    mov ebx,160
    div ebx
    add edi,edx
    pop edx
    pop ebx
    pop eax
    inc esi
    jmp .read_str
.end:
    ;重新设置cursor
    mov eax,edi
    mov ebx,160
    div ebx ;eax行，edx为列
    shr edx,1
    push eax
    push edx
    call SetCursor
    add esp,8
    pop ebp
    ret
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
    mov dx,[ebp+8]
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
global get_cr2
get_cr2:
    ;参数1：目标地址
    push ebp
    mov ebp,esp
    push eax
    push edi
    mov edi,[ebp+8]
    mov eax,cr2
    mov [edi],eax
    pop edi
    pop eax
    pop ebp
    ret
    