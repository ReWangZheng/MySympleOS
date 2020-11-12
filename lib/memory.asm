;导出函数
global memcpy
;void* memcpy(void * des,void* source,int sizes); 函数原型
memcpy:
	;参数1：目标地址
	;参数2：源地址
	;参数3：复制大小
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push edi
	push esi
	mov edi,[ebp+8] ;参数1
	mov esi,[ebp+12];参数2
	mov ecx,[ebp+16];参数3
	cld
	rep movsb
	pop esi
	pop edi
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret
