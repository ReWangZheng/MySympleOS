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
void show_num(){
    int number = 0;
    while (1)
    {
        int i=0xffffffff;
        show_str_format(0,5,"process1 state:%d",number++);
    }
}
//见初始化一个进程s
Process kernel;
int __kernel__();
void cstart(){
    GDT old_gdt;
    GetGDT(&old_gdt);
    memcpy(&gdt_ptr,&old_gdt,6);
    LoadGDT(&gdt_ptr); //000000000003519a
    initProcesTab();
    InitProcess(&kernel,__kernel__); //初始化进程
    init_interrupt(); //初始化中断
    init_pagemode(); //初始化分页模式
}

