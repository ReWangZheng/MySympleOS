#ifndef MYMEMORY
#define MYMEMORY
#include "memory.h"
u32 * malloc(int len); //分配指定大小的地址空间
u32 * malloc_4096_align(); //分配一个4kb对齐大小的页空间
void memset(u8 *dst,u8 v,int len);
#endif
