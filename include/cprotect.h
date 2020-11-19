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
void SetUpGdtDescriptor(u32 des);
#endif