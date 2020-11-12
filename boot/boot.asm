	org 0x7c00
	jmp short LABEL_START		; Start to boot.
	nop				; 这个 nop 不可少
	%include "fat.inc" ;FAT的磁盘头
	message db 'hello,this is my os boot!'
	message_0 db 'successfully!'
	message_0_len equ $-message_0
	message_1 db 'faild'
	message_1_len equ $-message_1
	FAT_base dw 0x0950
	FAT_offset equ 0
	loader_base dw 0x1000 ;段地址
	loader_offset dw 0x000 ;偏移地址
	loader_name db 'LOADER  BIN'
LABEL_START:
	mov ax,.read_end
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0x7c00
	;清屏
	mov ax,0x600
	mov bx,0x700
	mov cx,0
	mov dx,0x184f
	int 10h

	push word 25
	push word 0x00
	push message
	push ds
	call show_str
	
	push word 0x01
	mov ax,[loader_offset]
	push ax 
	mov ax,[loader_base]
	push ax
	push word 19 ;先读取FAT12文件系统的根目录
	call read_sector
	;下面开始在内存中搜索LOADER
	mov ax,[loader_offset]
	mov di,ax
	mov ax,[loader_base]
	mov es,ax
	
	mov si,loader_name
	xor bx,bx

.cmp_process:
	mov al,[es:di] 
	mov ah,[ds:si+bx]
	cmp al,ah
	jnz .next1
	inc bx
	inc di
	jmp	.cmp_process
.next1:
	cmp bx,11
	jz .success
	cmp di,520*14
	jz .fail
	xor bx,bx
	inc di
	jmp .cmp_process
.success:
	mov ax,message_0_len
	push ax
	push word 0x0100
	push message_0
	push cs
	call show_str
.read_lodaer:
	add di,15	
	
	mov si,[loader_offset]
	
	push word 0x01 ;读的扇区数量
	push si ;偏移量
	push word [loader_base] ;段地址
	mov ax,[es:di] ;得到起始簇号
	mov bx,ax
	add ax,31
	push ax;起始扇区号
	call read_sector
.read_FAT:
	push bx
	call NextFATEntry ;读下一个簇号,此时ax中保存的即为
	cmp ax,0xfff
	jz .read_end
	mov bx,ax
	add si,512
	mov ax,1
	push ax ;读的扇区数量
	push si ;偏移量
	push word [loader_base] ;段地址
	mov ax,[es:di] ;得到起始簇号
	mov ax,bx
	add ax,31
	push ax;起始扇区号
	call read_sector
	jmp .read_FAT
.read_end:
	jmp 0x1000:0x00
	hlt
.fail:
	push word message_1_len
	mov ax,0x1000
	push ax
	push message_1
	push cs
	call show_str
	hlt
NextFATEntry:
	;参数1：给出一个簇号，求出其下一个簇
	;返回：ax为下一个簇号
	mov bp,sp
	push bx
	push cx
	push dx
	push es
	mov ax,[ss:bp+2] ;参数1
	;第一步，求出个该簇在FAT表中的偏移位置
	xor dx,dx
	mov bx,2
	div bx ;ax为商，dx为余数
	mov bl,3
	mul bl ;ax为结果,此时ax存储的为偏移位置
	push dx ;存储一下余数
	xor dx,dx
	mov bx,512
	div bx ;此时ax存储的即为扇区数，dx为余数
	inc ax
	mov bx,ax;bx存储的即为起始扇区
	pop cx
	
	mov ax,2
	push ax ;参数4
	mov ax,FAT_offset
	push ax ;参数3
	push word [FAT_base] ;参数2
	push bx ;参数1
	call read_sector
	;读完之后，我们接下来要定位到那三个字节,即保存在dx中
	mov ax,[FAT_base]
	mov es,ax
	mov bx,dx
	mov eax,[es:bx] ;一次读了4个字节，但是只有前面3个有用
	;在判断余数是否为0
	cmp cx,0
	jz .zero_yushu
.one_yuhsu:
	;如果余数不为0,则取后面的1.5个字节
	shr eax,12
	and eax,0x0fff
.zero_yushu:
	;如果余数为0，则只取前面的1.5个字节
	and eax,0x0fff
	pop es
	pop dx
	pop cx
	pop bx

	ret 2
		
;------show_str
show_str:
	;参数1：串的段地址
	;参数2：串的偏移地址
	;参数3：显示坐标
	;参数4：串的长度
	mov bp,sp
	push es
	push di
	mov ax,[ss:bp+0x02] ;参数1
	mov es,ax
	mov dx,[ss:bp+0x06] ;参数3
	mov cx,[ss:bp+0x08] ;参数4
	mov bp,[ss:bp+0x04] ;参数2
	mov ax,0x1301
	mov bx,0007h		
	int 10h
	pop di
	pop es
	ret	8
;read_sector 读取指定扇区
read_sector:
	;参数1 扇区号
	;参数2 缓存区段地址
	;参数3 缓存区偏移地址
	;参数4 读取的扇区的数量
	mov bp,sp
	push ax
	push bx
	push cx
	push dx
	push es
	
	mov ax,[ss:bp+2] ;参数1
	;下面我们要计算我们即将读取的柱面、扇区、磁头
	xor dx,dx
	mov bx,18;每个磁道的扇区数
	div bx; ax为商，dx为余数
	
	add dx,1 ;在磁道上的扇区号
	mov cl,dl
	
	mov bx,ax
	shr bx,1 ;磁道号
	mov ch,bl
	
	and ax,1 ;读头号
	mov dh,al
	mov dl,0
	
	
	
	mov ax,[ss:bp+4] ;参数2
	mov es,ax
	mov bx,[ss:bp+6] ;参数3
	
	mov ax,[ss:bp+8] ;参数4
	mov ah,0x02
	int 13h
	pop es
	pop dx
	pop cx
	pop bx
	pop ax
	ret 8
times 510-($-$$) db 0
dw 0xaa55
	
	
	

