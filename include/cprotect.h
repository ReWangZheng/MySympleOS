#ifndef CPROTECT
#define CPROTEXT

//注册一个GDT描述符
void AddGDTDes();

//创建一个GDT描述符
void MakeGDTDes();

//初始化8259芯片
void Init_8259();

// 安装一个中断处理程序
void SetInt(int num,u32 addr);

//安装一个GDT描述符
u32 SetUpGdtDescriptor(Descriptor des);
u32 SetUpLdtDescriptor(LDT *ldt,Descriptor des,u16 ldtsel);

//制作一个描述符
Descriptor make_descriptor(u32 address,u32 limit,u16 attr);


#endif