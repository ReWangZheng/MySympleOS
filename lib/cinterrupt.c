#include "Interrupt.h"
#include "util.h"
#include "const.h"
unsigned char times=0;
unsigned int cur = 0;
void  CInterrupt_0(){
    times++;
    char temp[15];
    if(times==0){
        display_str(itoc(temp,cur++),0,10);
    }
    
}