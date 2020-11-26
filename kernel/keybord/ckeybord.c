#include "type.h"
#include "keybord.h"
#include "cmemory.h"
#include "buffer.h"
#include "util.h"
#include "ctime.h"
#define KEYBUFF_MAXLEN 200
Buffer *key_buffer;
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

void KeyProcess_enter(){
    InitKeyBuf();
    int number = 0;
    while (1)
    {
        KeyCode code = fetch_code();
        if(code==0){
            continue;            
        }
        show_str_format(0,4,"%x ",code);
        
        number+=5;
    }
}

