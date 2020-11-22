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
extern u32 getEFLAGE();
void show_num(){
    int number = 0;
    while (1)
    {
        show_str_format(0,5,"process1 state:%d",number++);
    }
    
}
void show_num2(){
    int number = 0;
    while (1)
    {
        show_str_format(0,6,"process2 state:%d",number++);
    }                                                                                                                                                                                              
}
//见初始化一个进程s
Process p1;
Process p2;
void cstart(){
    GDT old_gdt;
    GetGDT(&old_gdt);
    memcpy(&gdt_ptr,&old_gdt,6);
    LoadGDT(&gdt_ptr); //000000000003519a
    initProcesTab();
    InitProcess(&p2,show_num2);
    InitProcess(&p1,show_num);
    init_interrupt(); //0x000000035012
    init_pagemode();
}

