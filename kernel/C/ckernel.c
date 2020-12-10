#include "summary.h"
#include "cerror.h"
#include "ctty.h"
Process keybod_process;
Process hd_process;
Process tty_p;
extern PartInfor parts[10];
void __kernel__(){
    initDestop();
    int number = 0;
    RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    RunProcess(&hd_process,hd_server,0); //测试进程能否返回成功！
    RunProcess(&tty_p,TTY_Process,0);
    Sector s;
    ReadHardDiskMes(0);
    for(int i =0;i<512;i++){
        s.data[i] = 0xff;
    }
    WriteHDLinear(0x3000,&s,200);
    MakeFileSystem(parts[2]); //制作文件系统
    do_mkfile("C:","disk.txt",ATTR_TXT);
    do_mkfile("C:","hello.c",ATTR_TXT);

    do_open("C:/hello.c",1);

    while (1){
        show_str_format(0,13,"the kernel task is running :%d",number++);
        sleep(1000);
    }
}