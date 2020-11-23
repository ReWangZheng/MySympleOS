#include "Interrupt.h"
#include "util.h"
#include "const.h"
#include "protect.h"
#include "process.h"
unsigned char times=0;
unsigned int cur = 0;
extern IDT idt_ptr;
extern PCB process_tab;
int num=0;
void  CInterrupt_0(){
    show_str_format(0,20,"Interrupt 0 :%d",num++);
}

void init_interrupt(){
        //设置idt
    idt_ptr.idt_len = IDT_LEN;
    idt_ptr.idt_low_addr = IDT_ADDR;
    idt_ptr.idt_high_addr = (IDT_ADDR>>16);
    LoadIDT(&idt_ptr); //加载IDT
    SetInt(0,DE_ERR);
    SetInt(10,TS_ERR);
    SetInt(11,NP_ERR);
    SetInt(12,SS_ERR);
    SetInt(13,GP_ERR);
    SetInt(14,PF_ERR);
    Init_8259();
    SetInt(0x20,Interrupt_0);
}

void exception_handle(int code){
    char *mes[7]={
    " ",
    "The divisor cannot be 0!",
    "TSS segment  valid",
    "Segment valid",
    "Stack Segment err",
    "the memeory or protect mode check err ",
    "page err!"
    };
    show_str_format(0,1,"err message: %s",mes[code]);
    if(code==6){
        u32 addr;
        get_cr2(&addr);
        AddPageOnadd(addr);
    }
}