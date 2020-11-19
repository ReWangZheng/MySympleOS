#include "const.h"
#include "type.h"
#include "util.h"
#include "cprotect.h"
#include "cmemory.h"
#include "Interrupt.h"
#include "page.h"
#include "protect.h"
GDT gdt_ptr;
IDT idt_ptr;
int gdt_size=0;
void cstart(){
    GDT old_gdt;
    GetGDT(&old_gdt);
    memcpy(&gdt_ptr,&old_gdt,6);
    LoadGDT(&gdt_ptr); //000000000003519a
    SetUpGdtDescriptor(0x00000000); //0x351ae
    idt_ptr.idt_len = IDT_LEN;
    idt_ptr.idt_low_addr = IDT_ADDR;
    idt_ptr.idt_high_addr = (IDT_ADDR>>16);
    LoadIDT(&idt_ptr);
    init_interrupt(); //0x000000035012
}

