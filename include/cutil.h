#ifndef CUTIL
#define CUTIL
#include "type.h" 
#include "util.h"
//void SetCursor(int x,int y);
Cursor GetCursor();
void print(char *str);
u16 u8_and_u8(u8 high,u8 low);
u32 u16_and_u16(u16 high,u16 low);
char * itoc(char *str,int number);
void clean_screen();
#endif