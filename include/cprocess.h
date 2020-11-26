#ifndef CPROCESS
#define CPROCESS
#include "type.h"

typedef struct Process
{
    //popad
    u32 EDI;
    u32 ESI;
    u32 EBP;
    u32 ESP;
    u32 EBX;
/*5*/ 
    u32 EDX;
    u32 ECX; // <----esp-4
    u32 EAX; // <----esp指向的位置
    u32 EIP;
    u32 CS;
/*10*/ 
    u32 EFLAGE;    
    TSS *tss; // <----esp+4
    u32 tss_sel; 
    LDT *ldt;
    u32 ldt_sel; //只用低16位
/*15*/  
    int PID;
    //页目录
    u32 * page_catalog;
    //段寄存器
    u32 ds;
    u32 es;
    u32 ss;
/*20*/  
    u32 fs;
    u32 gs;
    u32 DPL;
    //下一个进程
    struct Process * next;
}Process;


typedef struct PCB
{
    int p_size;
    Process *head;
    Process *cursor;
    TSS * tss;
}PCB;

void initProcesTab();
void AddProcess(Process *p);
Process * fetch();
void  RunProcess(Process * p,void * enter,u32 DPL);
Process * getCurrentP();
void initTSS(u32 ss0,u32 esp0,u32 ss1,u32 esp1,u32 ss2,u32 esp2);
void InitKernelProcess(Process * p,void * enter);//初始化内核进程

#endif
