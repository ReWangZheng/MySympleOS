#include "process.h"
#include "util.h"
#include "ctime.h"
#include "keybord.h"
#include "const.h"
#include "page.h"
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
    RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    int number = 0;
    while (1){
        sleep(1000);
        show_str_format(0,15,"the kernel task is running : %d",number++);
    }
}