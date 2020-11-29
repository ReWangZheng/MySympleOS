#include "cmemory.h"
extern u32 start_allocate;
//分配内存空间
u32 * malloc(int len){
    u32 *res = start_allocate;
    (*res) = 0;
    start_allocate = start_allocate + (len);
    return res;
}

u32 * malloc_4096_align(){
    u32 mo = start_allocate % 4096;
    u32 res = start_allocate &0xfffff000;
    if(mo>0){
        start_allocate =res + 0x1000;
    }
    u32 * r =(u32*)start_allocate;
    start_allocate = start_allocate + 0x1000;
    (*r) = 0;
    return r;
}

void read(char * filename,u8 dst[],int len){

}