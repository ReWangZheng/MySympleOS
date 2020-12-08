#ifndef CTTY
#define CTTY

#include "type.h"
#include "buffer.h"
#include "keyboard.h"
#define MAX_TTY 3
typedef struct console{
    u32  dis_start; //当前控制台显存的起始位置
    u32  append_addr; //当前控制台显示的位最
    int cursor;
};

typedef struct tty{
    int id;
    Buffer *Buffer;
    struct console * cs;
};

struct tty * getCurrentTTY();
void TTY_Process();
void switch_tty(int tty_id);
#endif