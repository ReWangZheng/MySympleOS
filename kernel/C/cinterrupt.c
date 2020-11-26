#include "Interrupt.h"
#include "util.h"
#include "const.h"
#include "protect.h"
#include "process.h"
#include "keybord.h"
unsigned char times=0;
unsigned int cur = 0;
extern IDT idt_ptr;
extern PCB process_tab;
int num=0;
int ticks;
// 时钟中断处理函数
void  CInterrupt_0(){
    ticks++;
}
// 键盘中断处理汉函数
void keybordHandle(){
    //得到键盘扫描码
    KeyCode code = in_byte(0x60);
    append_code(code);
    show_str_format(0,7,"%d",keybuffersize());
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
    //为时钟中断绑定函数
    SetInt(0x20,Interrupt_0);
    //为键盘中断绑定处理函数
    SetInt(0x21,keybordInt);
    
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