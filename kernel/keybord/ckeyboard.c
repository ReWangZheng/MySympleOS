#include "summary.h"
#include "keymap.h"
#define KEYBUFF_MAXLEN 200
Buffer *key_buffer;
int t;
int SHIFT_PRESS = 0;
int CAPSLOCKS = 0;
void InitKeyBuf(){
    key_buffer = MakeBuffer(KEYBUFF_MAXLEN);
}

void append_code(KeyCode code){
    append(key_buffer,code);
}
KeyCode fetch_code(){
    return get(key_buffer);
}

int keybuffersize(){
    return key_buffer->size;
}
void handle(KeyCode code){
    char str[2] = " ";
    char key_ch = 0;
    if(code==0xE1){
        //暂时不做任何操作
    }else if(code==0xE0){
        //也不做操作
    }else{
        //如果是make码
        char key[]="key:";
        show_str_format(0,5,key);
        if((0x80 & code)==0){
            u32 key = keymap[(code&0x7f)*3];
            if(key==SHIFT_L){
                SHIFT_PRESS = 1;
            }else if(key==CAPS_LOCK){
                CAPSLOCKS = !CAPSLOCKS;
            }else{
                key_ch = keymap[(code&0x7f)*3+SHIFT_PRESS];
                if(CAPSLOCKS&&((key_ch>'a' && key_ch<'z')||(key_ch>'A'&&key_ch<'Z'))){
                    if((key_ch&32)==32){
                        key_ch = key_ch-32; 
                    }else{
                        key_ch = key_ch+32;
                    }
                }
                str[0] = key_ch;
                show_str_format(t++,5,"%s",str);
            }
        }else if((0x80 & code)==0x80){
            //如果是break码
            u32 key = keymap[(code&0x7f)*3];
            //如果SHIFT键被松开
            if(key==SHIFT_L){
                SHIFT_PRESS =0;
            }

        }
    }
}
void KeyProcess_enter(){
    InitKeyBuf();
    t = 4;
    SHIFT_PRESS = 0;
    CAPSLOCKS = 0;
    while (1)
    {
        KeyCode code = fetch_code();
        if(code==0){
            continue;            
        }
        handle(code);
    }
}


