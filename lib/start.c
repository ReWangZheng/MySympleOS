#include "const.h"
#include "type.h"
#include "util.h"
GDT gdt_ptr;
int gdt_size=0;
void cstart(){
    gdt_ptr.gdt_len = 48;
    gdt_ptr.gdt_low_addr = 0x7000;
    gdt_ptr.gdt_high_addr = 0x0003;
}