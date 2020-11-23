#ifndef PAGE
#define PAGE
#include "type.h"
#include "cpage.h"
void open_pagemode(u32 page_addr); //开启分页模式,并且载入页目录
void loda_page_catelog(u32 addr); //载入页目录
#endif