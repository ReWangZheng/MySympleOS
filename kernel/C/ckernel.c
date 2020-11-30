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
Process p;
Process p2;
Process p3;
Process p4;
Process p5;

void __kernel__(){
    //RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    RunProcess(&p,show3,0); //测试进程能否返回成功！
    RunProcess(&p2,show4,0); //测试进程能否返回成功！
    RunProcess(&hd_process,hd_server,0);
    RunProcess(&p3,show5,0);
    RunProcess(&p4,show6,0);
    RunProcess(&p5,show7,0);
    int number = 0;
    char str = "the kernel task is running :%d";
    while (1){
        show_str_format(0,13,str,number++);
        //getCurrentP()->state = Sending;
    }
}

void show3(){
    int number = 100;
    while (number>5)
    {
        show_str_format(0,8,"process p:%d",number++);
    }
}
void show4(){
    int number = 50;
    while (number>5)
    {
        show_str_format(0,9,"process p:%d",number++);
    }
}
void show5(){
    int number = 50;
    while (number>5)
    {
        show_str_format(0,10,"process p:%d",number++);
    }}
void show6(){
    int number = 50;
    while (number>5)
    {
        show_str_format(0,11,"process p:%d",number++);
    }}
void show7(){
    int number = 50;
    while (number>5)
    {
        show_str_format(0,12,"process p:%d",number++);
    }
    }