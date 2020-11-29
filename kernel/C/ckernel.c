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
    while (number<=5)
    {
        u8 * add = 0xa0000;
        for(int j=0;j<0xffff;j++){
            *(add+j)=15;
            sleep(1);
        }
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