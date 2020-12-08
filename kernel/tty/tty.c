#include "ctty.h"
#include "util.h"
struct console consoles[MAX_TTY];
struct tty ttys[MAX_TTY];
int current_TTY;
#define Delete_key 0x4
void initTTY(){
    current_TTY = 0;
    int start_addr = 0xb8000;
    int dis_size = 0x2800;
    for(int i =0;i<MAX_TTY;i++){
        ttys[i].Buffer = MakeBuffer(100);
        ttys[i].cs = &consoles[i];
        consoles[i].cursor = 0;
        consoles[i].append_addr=0;
        consoles[i].dis_start=start_addr+ dis_size * i;
    }
}

// TTY_task
void TTY_Process(){
    initTTY();
    //show_str_format(0,2,"TTY_TASK RUNNING!");
    while (1)
    {
        struct tty *stty = getCurrentTTY();
        struct console *cs = stty->cs;
        Buffer *buf = stty->Buffer;
        for(int k =0;k<buf->size;k++){
            u16 *addr = cs->dis_start + cs->cursor*2;
            u8 c = get(buf);
            // 如果C是退格键盘
            if(c==Delete_key){
                //*addr=u8_and_u8(0x07,0);
                left_shift();
                cs->cursor--;
                cs->append_addr-=2;
            }else if(c==0x17){
                // 如果C是LEFT
                if(cs->cursor>0){
                    cs->cursor--;
                }
            }else if(c==0x18){
                // 如果C
                if(cs->cursor*2<cs->append_addr){
                    cs->cursor++;
                }
            }else{
                right_shift();
                *addr=u8_and_u8(0x07,c);
                addr++;
                cs->append_addr+=2;
                cs->cursor++;
            }
            updatecursor();
            show_str_format(0,2,"%x",c); // 退格 0x43
        }
    }
}

struct tty * getCurrentTTY(){
    return &ttys[current_TTY];
}

void switch_tty(int tty_id){
    current_TTY = tty_id;
    struct tty * stty = getCurrentTTY();
    int start = (stty->cs->dis_start-0xb8000)/2;
    // 低8位
    out_byte(0x3d4,0xd);
    out_byte(0x3d5,(u8)start);
    // 高8位
    out_byte(0x3d4,0xc);
    out_byte(0x3d5,(u8)(start>>8));
    updatecursor();
}

void updatecursor(){
    struct console *cs= getCurrentTTY()->cs;
    unsigned int cursor = cs->cursor + 0x1400 * current_TTY;
    out_byte(0x3d4,0xf);
    out_byte(0x3d5,cursor);
    out_byte(0x3d4,0xe);
    out_byte(0x3d5,cursor>>8);
}


void right_shift(){
    struct console *cs= getCurrentTTY()->cs;
    int dis_end = cs->dis_start + cs->append_addr;
    int dis_start = cs->dis_start + cs->cursor * 2;
    u16 * pre = cs->dis_start + cs->cursor*2;
    u16 * current = cs->dis_start + (cs->cursor+1)*2;
    for(;current <= cs->dis_start+cs->append_addr;current++,pre++){
        *current=*pre;
    }
}

void left_shift(){
    struct console *cs= getCurrentTTY()->cs;
    u16 * current = cs->dis_start + (cs->cursor-1)*2;
    u16 * back = current+1;
    u16 * end = cs->dis_start + cs->append_addr;
    for(;current<end;current++,back++){
        *current = *back;
    }
}