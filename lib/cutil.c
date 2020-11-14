#include "cutil.h"
#include "type.h"
Cursor GetCursor(){
    Cursor cursor;
    u16 cursors = getcursor();
    cursor.x = cursors % 80;
    cursor.y = cursors / 80;
    return cursor;
}

void print(char *str){
    Cursor cursor=GetCursor();
    display_str(str,cursor.x,cursor.y);
}

u16 u8_and_u8(u8 high,u8 low){
    u16 res = ((u16)high<<8)|((u16)low);
    return res;
}
u32 u16_and_u16(u16 high,u16 low){
    u32 res = ((u32)high<<16)|((u32)low);
    return res;
}

char * itoc(char *str,int number){
    static char numberchar[]={'0','1','2','3','4','5','6','7','8','9'};
    int i=0;
    int yushu =0;
    int len=0;
    int temp = number;
    do
    {
        temp /= 10; //10 1 0
        len++;
    } while (temp > 0);
    str[len]=0;
    do
    {
        yushu =number % 10;
        number /= 10;
        str[--len]=numberchar[yushu];
    } while (number > 0);
    return str;
}