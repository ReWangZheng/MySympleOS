#include "const.h"
#include "type.h"
#include "util.h"
#include "cprotect.h"
#include "cmemory.h"
#include "Interrupt.h"
#include "page.h"
#include "protect.h"
#include "process.h"
GDT gdt_ptr;
IDT idt_ptr;
int gdt_size=0;
Process kernel; //内核进程
Process p; //内核进程
void show(){
    int number = 0;
    while (1)
    {
        show_str_format(0,5,"process2:%d",number++);
    }
    
}
int __kernel__();
void SYSTERM_INIT(){
    GDT old_gdt;
    GetGDT(&old_gdt);
    memcpy(&gdt_ptr,&old_gdt,6);
    LoadGDT(&gdt_ptr); //000000000003519a
    init_interrupt(); //初始化中断
    init_pagemode(); //初始化分页模式
    initProcesTab(); //初始化进程表
    InitProcess(&kernel,__kernel__,DPL_0); //初始化进程
    InitProcess(&p,show,DPL_0);
}

