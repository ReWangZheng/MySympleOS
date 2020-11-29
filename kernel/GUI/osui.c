#include "osui.h"
#include "type.h"
void initDestop(){  
    u8 * vga = 0xa0000;
    for(int i =0;i<65535;i++){
        (*vga) = 5;
        vga++;
    }


}