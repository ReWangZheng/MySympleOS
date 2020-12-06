#ifndef UTIL
#define UTIL
#include "type.h"
#include "cutil.h"
void SetCursor(int x,int y);
u16 getcursor();
void display_str(char *str,int x,int y);
void out_byte(u16 port,u8 value);
u8 in_byte(u16 port);
u16 in_byte_16(u16 port);
void out_byte_16(u16 port,u16 value);
void clear();
void get_cr2(u32 *addr);
void ud2_indicate();
#endif