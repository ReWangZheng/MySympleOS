#include "process.h"
#include "util.h"
#include "ctime.h"
#include "ckeyboard.h"
#include "const.h"
#include "page.h"
#include "protect.h"
#include "harddisk.h"
#include "ckernel.h"
Process keybod_process;
Process hd_process;
void __kernel__(){
    //RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    RunProcess(&hd_process,hd_server,0); //测试进程能否返回成功！
    int number = 0;
    char *str = "the kernel task is running :%d";
    // Sector s;
    // ReadHDLBA(0,&s,1);
    // char c[]="add : %x";
    // show_str_format(0,14,c,s.data[0x1c2]);
    ReadHardDiskMes(0);
    while (1){
        show_str_format(0,13,str,number++);
    }
}