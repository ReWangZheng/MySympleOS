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
char * itoh(char *str,int number);
int bit_test(u8 input,int flag);
void clean_screen();
void show_str_format(int x,int y,const char *str,...);
int strcmp(char * dst,char * src);
void strcp(char * dst,char * src);
int split(char * src,char splitch,char buf[][16],int max);
int bit(u8 *buf,int x);
#endif