%macro Descriptor 3 ;段基地址、段界限、段属性
	dw %2&0xffff;0-15位，即段界限
	dw %1&0xffff;16-31位,段基地址
	db (%1>>16)&0xff;32-39位，段基地址
	dw ((%2>>8)&0xf00)|(%3 & 0xf0ff);属性
	db (%1>>24) & 0xff;段基址
%endmacro
