#include "const.h"
#include "type.h"
#include "util.h"
#include "cprotect.h"
#include "cmemory.h"
#include "Interrupt.h"
#include "page.h"
GDT gdt_ptr;
IDT idt_ptr;
int gdt_size=0;
void cstart(){
    GDT old_gdt;
    GetGDT(&old_gdt);
    memcpy(&gdt_ptr,&old_gdt,6);
    idt_ptr.idt_len = IDT_LEN;
    idt_ptr.idt_low_addr = IDT_ADDR;
    idt_ptr.idt_high_addr = (IDT_ADDR>>16);
    init_interrupt(); //0x000000035012
}