#include "process.h"
#include "util.h"
#include "ctime.h"
#include "keybord.h"
#include "const.h"
#include "page.h"
#include "Interrupt.h"
#include "protect.h"
Process keybod_process;
Process p;
void show3(){
    int number = 0;
    while (1)
    {
        show_str_format(0,3,"process p:%d",number++);
    }
}
int __kernel__(){
    show_str_format(0,15,"the kernel task start running ");

    InitKeyBuf();
    close_int();
    RunProcess(&p,show3,0);
    open_int();
    int number = 0;
    while (1){
        sleep(1000);
        show_str_format(0,15,"the kernel task is running : %d",number++);
    }
}