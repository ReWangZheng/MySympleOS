#ifndef TYPE
#define TYPE
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
//描述符，共8个字节
typedef struct Descriptor
{
    u32 low;
    u32 high;
}Descriptor;
typedef struct Int_Descriptor
{
    u16 low_off;
    u16 code_sel;
    u16 attr;
    u16 high_off;
}Int_Descriptor;

typedef struct GDT
{
    u16 gdt_len; //gdt的长度
    u16 gdt_low_addr; //gdt的低地址
    u16 gdt_high_addr; //gdt高地址
}GDT;

typedef struct LDT
{
    u16 ldt_len; //gdt的长度
    u16 ldt_low_addr; //gdt的低地址
    u16 ldt_high_addr; //gdt高地址
}LDT;

typedef struct IDT
{
    u16 idt_len; 
    u16 idt_low_addr; 
    u16 idt_high_addr; 
}IDT;


typedef struct Cursor
{
    int x;
    int y;
}Cursor;

typedef struct TSS
{
    u32 * tss_pre;
    u32 ESP0;
    u32 SS0_sel;
    u32 ESP1;
    u32 SS1_sel;
    u32 ESP2;
    u32 SS2_sel;
    u32 CR3;
    u32 EIP;
    u32 EFLAGE;
    u32 EAX;
    u32 ECX;
    u32 EDX;
    u32 EBX;
    u32 ESP;
    u32 EBP;
    u32 ESI;
    u32 EDI;
    u32 ES_sel;
    u32 CS_sel;
    u32 SS_sel;
    u32 DS_sel;
    u32 FS_sel;
    u32 GS_sel;
    u32 LDT_sel;
    u32 IOOP;
}TSS;


typedef  u8 KeyCode;


#endif
