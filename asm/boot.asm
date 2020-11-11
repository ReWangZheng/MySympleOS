	org 0x7c00
	jmp short LABEL_START		; Start to boot.
	nop				; 这个 nop 不可少
	; 下面是 FAT12 磁盘的头
	BS_OEMName	DB 'ForrestY'	; OEM String, 必须 8 个字节
	BPB_BytsPerSec	DW 512		; 每扇区字节数
	BPB_SecPerClus	DB 1		; 每簇多少扇区
	BPB_RsvdSecCnt	DW 1		; Boot 记录占用多少扇区
	BPB_NumFATs	DB 2		; 共有多少 FAT 表
	BPB_RootEntCnt	DW 224		; 根目录文件数最大值
	BPB_TotSec16	DW 2880		; 逻辑扇区总数
	BPB_Media	DB 0xF0		; 媒体描述符
	BPB_FATSz16	DW 9		; 每FAT扇区数
	BPB_SecPerTrk	DW 18		; 每磁道扇区数
	BPB_NumHeads	DW 2		; 磁头数(面数)
	BPB_HiddSec	DD 0		; 隐藏扇区数
	BPB_TotSec32	DD 0		; wTotalSectorCount为0时这个值记录扇区数
	BS_DrvNum	DB 0		; 中断 13 的驱动器号
	BS_Reserved1	DB 0		; 未使用
	BS_BootSig	DB 29h		; 扩展引导标记 (29h)
	BS_VolID	DD 0		; 卷序列号
	BS_VolLab	DB 'OrangeS0.02'; 卷标, 必须 11 个字节
	BS_FileSysType	DB 'FAT12   '	; 文件系统类型, 必须 8个字节  

	message db 'hello,this is my os boot!'
	loader_base dw 0x1000 ;段地址
	loader_offset dw 0x000 ;偏移地址
LABEL_START:
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
	mov ax,25
	push ax
	mov ax,0x00
	push ax
	push message
	push ds
	call show_str
	
	
	mov ax,1
	push ax
	mov ax,[loader_offset]
	push ax 
	mov ax,[loader_base]
	push ax
	mov ax,19 ;先读取FAT12文件系统的根目录
	push ax
	call read_sector

	
	;参数4:串长度
	mov ax,16
	push ax
	
	;参数4:显示坐标
	mov ax,0x0200;
	push ax
	;7cab
	;参数4: 偏移地址
	push word [loader_offset]
	;参数4：段地址
	push word [loader_base]
	call show_str
	hlt

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
	shr bx,2 ;磁道号
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
	pop dx
	pop cx
	pop bx
	pop ax
	pop es
	ret 8
times 510-($-$$) db 0
dw 0xaa55
	
	
	

