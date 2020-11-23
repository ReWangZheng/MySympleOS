#include "cmemory.h"
extern u32 start_allocate;
//分配内存空间
u32 * malloc(int len){
    u32 *res = start_allocate;
    (*res) = 0;
    start_allocate = start_allocate + (len);
    return res;
}