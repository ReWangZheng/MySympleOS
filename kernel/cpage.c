#include "page.h"
#include "const.h"
#include "memory.h"
#include "util.h"
#define PAGDRR(x) ((u32)(x))&0xfffff000
extern u32 * page_catalog;//
extern u32 * page_table_4MB;

u8 page_bitmap[16384]; // 0x38060
int bitmap_cursor;
void init_bitmap(){
    bitmap_cursor = 256;
    for(int i = 0;i<32;i++){
        page_bitmap[i] = 0xff;
    }
    for(int i = 32;i<16384;i++){
        page_bitmap[i] = 0x00;
    }
}
u32 allocate_page(){
    while (1)
    {   
        if(IsAllocated(bitmap_cursor))
            bitmap_cursor = (bitmap_cursor+1) % 131072;
        else{
            u32  address= (0x1000 * bitmap_cursor) | PAGE__ATTR;
            SetAllocated(bitmap_cursor++,1);
            return address;
        }
            
    }    
}

void AddPageOnadd(u32 addr){
    //得到页的数
    int pagecount = addr / 4096; 
    //得到页在table中的index
    int table_index = pagecount / 1024;
    //得到在页表中的page_index
    int page_index = pagecount % 1024;
    //先判断一下目录表是否为NULL
    if(page_catalog[table_index]==0){
        u32  tab = allocate_page();
        u32 *tab_addr = ((u32)0xFFFFF000+(u32)(table_index*4));
        (*tab_addr) = PAGDRR(tab) | PAGE_TABLE_ATTR;
    }
    //得到页目录的线性地址
    u32 * page_addr = (u32*)(0xFFC00000+(table_index<<12)+page_index*4);
    //得到一个分配的页表
    u32 addr_allocated = allocate_page();
    (*page_addr) = PAGDRR(addr_allocated) | PAGE__ATTR; 
}

//判断页是否被分配
int IsAllocated(int cursor){
    int byte_index = cursor / 8;
    int bit_index = cursor % 8;
    u8 byte_temp = page_bitmap[byte_index];
    u8 andres = byte_temp&(1<<bit_index);
    if(andres > 0){
        return 1;
    }else{
        return 0;
    }
}

//设置Bitmap
void SetAllocated(int cursor,int flag){
    int byte_index = cursor / 8;
    int bit_index = cursor % 8;
    u8 byte_temp = page_bitmap[byte_index];
    if(flag == 0){
        u8 m1 = 0xff;
        u8 m2 = 1<<bit_index;
        u8 mask = m1^m2;
        u8 andres = byte_temp&mask;
    }else{
        u8 mask = (1<<bit_index);
        page_bitmap[byte_index] = page_bitmap[byte_index] | mask;       
    }
}

//开启分页模式
void init_pagemode(){
    for(int j=0;j<1024;j++){
        page_catalog[j] = 0x00;
    }
    for(int i=0;i<1024;i++){
        if(i<256)
            page_table_4MB[i] = PAGDRR(0x1000*i) | PAGE__ATTR;
        else
            page_table_4MB[i] = 0x00;
    }
    page_catalog[0] = PAGDRR(page_table_4MB) | PAGE_TABLE_ATTR;
    page_catalog[1023] = PAGDRR(page_catalog) | PAGE_TABLE_ATTR;
    init_bitmap();
    open_pagemode(PAGDRR(page_catalog));
}