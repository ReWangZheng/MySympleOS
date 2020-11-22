#ifndef CPROCESS
#define CPROCESS
#include "type.h"

typedef struct Process
{
    u32 EDI;
    u32 ESI;
    u32 EBP;
    u32 ESP;
    u32 EBX;
    u32 EDX;
    u32 ECX; // <----esp-4
    u32 EAX; // <----esp指向的位置
    u32 EIP;
    u32 CS;
    u32 EFLAGE;
    TSS tss; // <----esp+4
    u32 tss_sel; 
    LDT ldt;
    u32 ldt_sel; //只用低16位
    int PID;
    struct Process * next;
    void * debug;
}Process;
typedef struct PCB
{
    int p_size;
    Process *head;
    Process *cursor;
}PCB;

void initProcesTab();
void AddProcess(Process *p);
Process * fetch();
void  InitProcess(Process * p,void * enter);

#endif
