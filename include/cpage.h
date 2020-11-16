#ifndef CPAGE
#define CPAGE
void init_pagemode(); //0x35874
void init_bitmap();
u32 allocate_page();
void AddPageOnadd(u32 addr);
int IsAllocated(int cursor);
#endif