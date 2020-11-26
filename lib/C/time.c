#include "ctime.h"
#include "util.h"
extern unsigned int ticks;
unsigned int getticks(){
    return ticks;
}
void sleep(u32 t){
    u32 t0 = getticks();
    while (getticks()-t0<t){
    }
    
}