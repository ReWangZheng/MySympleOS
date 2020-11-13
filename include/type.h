#ifndef TYPE
#define TYPE
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
//描述符，共8个字节
typedef struct S_Descriptor
{
    u16 limit_low; //2个字节
    u16 base_low; //2个字节
    u8 base_mid; // 1个字节
    u8 attr1; //1个字节
    u8 limit_high_att2; // 1个字节
    u8 base_high; //1个字节
}S_Descriptor;
typedef struct GDT
{
    u16 gdt_len; //gdt的长度
    u16 gdt_low_addr; //gdt的低地址
    u16 gdt_high_addr; //gdt高地址
}GDT;
typedef struct Cursor
{
    int x;
    int y;
}Cursor;

#endif
