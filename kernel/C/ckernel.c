#include "process.h"
#include "util.h"
#include "ctime.h"
#include "ckeyboard.h"
#include "const.h"
#include "page.h"
#include "protect.h"
Process keybod_process;
Process p;
void show3(){
    int number = 0;
    out_byte(0x3f6,0);
    out_byte(0x1f6,0xe0);
    out_byte(0x1f7,0xec);
    u8 state = in_byte(0x1f7);
    show_str_format(0,8,"process p:%x",state);
    while (number<=5)
    {
        sleep(1000);
    }
}
int __kernel__(){
    RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    RunProcess(&p,show3,0); //测试进程能否返回成功！
    int number = 0;
    while (1){
        sleep(1000);
        show_str_format(0,15,"the kernel task is running : %d",number++);
    }
}