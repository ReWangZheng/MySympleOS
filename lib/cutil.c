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
}//0000000000035030