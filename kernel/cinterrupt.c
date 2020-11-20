#include "Interrupt.h"
#include "util.h"
#include "const.h"
#include "protect.h"
unsigned char times=0;
unsigned int cur = 0;
void  CInterrupt_0(){
    //在切换前将现场首都保存下来

}

void init_interrupt(){
    SetInt(0,DE_ERR);
    SetInt(10,TS_ERR);
    SetInt(11,NP_ERR);
    SetInt(12,SS_ERR);
    SetInt(13,GP_ERR);
    SetInt(14,PF_ERR);
    Init_8259();
    SetInt(0x20,Interrupt_0);
}
char *mes[7]={
    " ",
    "The divisor cannot be 0!",
    "TSS segment  valid",
    "Segment valid",
    "Stack Segment err",
    "the memeory or protect mode check err ",
    "page err!"
};

void exception_handle(int code){
    print("err message: ");
    print(mes[code]);
    if(code==6){
        u32 addr;
        get_cr2(&addr);
        AddPageOnadd(addr);
    }
}