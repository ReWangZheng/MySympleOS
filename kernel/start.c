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
void cstart(){
    GDT old_gdt;
    GetGDT(&old_gdt);
    memcpy(&gdt_ptr,&old_gdt,6);
    LoadGDT(&gdt_ptr); //000000000003519a
    //见初始化一个进程s
    Process p;
    save_process_state(&p); //0x351c3
    //初始化ldt
    p.ldt.ldt_low_addr=0x0000;
    p.ldt.ldt_high_addr = 0x0004;
    p.ldt.ldt_len = 16;
    //制作ldt的描述符
    Descriptor ldt_des = make_descriptor(u16_and_u16(p.ldt.ldt_high_addr,p.ldt.ldt_low_addr),16,UNIT_1BY|P_OK|DPL_0|S_CODE|TYPE_LDT);
    //安装ldt
    p.ldt_sel = SetUpGdtDescriptor(ldt_des);
    
    //设置idt
    idt_ptr.idt_len = IDT_LEN;
    idt_ptr.idt_low_addr = IDT_ADDR;
    idt_ptr.idt_high_addr = (IDT_ADDR>>16);


    LoadIDT(&idt_ptr); //加载IDT
    LoadLDT(p.ldt_sel); //记载LDT
    init_interrupt(); //0x000000035012
    init_pagemode();
}

