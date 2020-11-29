#include "process.h"
#include "util.h"
#include "ctime.h"
#include "ckeyboard.h"
#include "const.h"
#include "page.h"
#include "protect.h"
#include "harddisk.h"
Process keybod_process;
Process hd_process;
Process p;
Process p2;

void show3(){
    int number = 100;
    while (number>5)
    {
        sleep(1000);

        show_str_format(0,8,"process p:%d",number++);
    }
}
void show4(){
    int number = 50;
    while (number>5)
    {
        sleep(1000);

        show_str_format(0,9,"process p:%d",number++);
    }
}
int __kernel__(){
    RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    RunProcess(&p,show3,0); //测试进程能否返回成功！
//    RunProcess(&p2,show4,0); //测试进程能否返回成功！
    RunProcess(&hd_process,hd_server,0);
    int number = 0;
    while (1){
        sleep(1000);
        show_str_format(0,15,"the kernel task is running : %d",number++);
    }
}