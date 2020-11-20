#include "util.h"
#include "const.h"
#include "type.h"
#include "memory.h"
#include "protect.h"
extern IDT idt_ptr;
extern GDT gdt_ptr;
void Init_8259(){
    out_byte(MASTER_8259_PORT1,ICW1);
    out_byte(CASCADE_8259_PORT1,ICW1);

    out_byte(MASTER_8259_PORT2,ICW2_MAST);
    out_byte(CASCADE_8259_PORT2,ICW2_CASCADE);
    
    out_byte(MASTER_8259_PORT2,ICW3_MAST);
    out_byte(CASCADE_8259_PORT2,ICW3_CASCADE);

    out_byte(MASTER_8259_PORT2,ICW4);
    out_byte(CASCADE_8259_PORT2,ICW4);

    //开启定时器中断
    out_byte(MASTER_8259_PORT2,OCW1_MAST);
    out_byte(CASCADE_8259_PORT2,OCW1_CASCADE);
}

void Init_RTC(){

}

void SetInt(int num,u32 addr){
    Int_Descriptor Idesc;
    Idesc.code_sel = Sele_Code_4GB;
    Idesc.low_off = addr;
    Idesc.high_off = (addr>>16);
    Idesc.attr = 0x8e00;
    u32 idt_address = u16_and_u16(idt_ptr.idt_high_addr,idt_ptr.idt_low_addr) + num*8;
    memcpy(idt_address,&Idesc,8);
}
u32 SetUpGdtDescriptor(Descriptor des){
    u32 * gdtaddr = u16_and_u16(gdt_ptr.gdt_high_addr,gdt_ptr.gdt_low_addr);
    gdtaddr += ((gdt_ptr.gdt_len+1) / 4);
    (*gdtaddr) = des.low;
    (*(gdtaddr+1)) = des.high;
    gdt_ptr.gdt_len+=8;
    LoadGDT(&gdt_ptr);
    u32 sel = ((-1+(gdt_ptr.gdt_len+1) / 8)<<3);
    return sel;
}

void SetUpLdtDescriptor(LDT *ldt,u32 des){
    u32 * ldtaddr = ((u32)ldt->ldt_high_addr<<16)|(ldt->ldt_low_addr);
    
    ldtaddr += ((gdt_ptr.gdt_len+1) / 8);
    (*ldtaddr) = des;
    ldt->ldt_len+=8;
    LoadLDT(ldt);
}

Descriptor make_descriptor(u32 address,u32 limit,u16 attr_){
    Descriptor res;
    u32 attr = (attr_<<8);
    res.low = (address<<16)|(limit&0x0000ffff);
    res.high = (address & 0xff000000)|(attr)|(limit&0x000f0000)|((address>>16)&0x00ff);
    return res;
}