#include "summary.h"
Process keybod_process;
Process hd_process;
extern PartInfor parts[10];

void __kernel__(){
    int number = 0;
    RunProcess(&keybod_process,KeyProcess_enter,0); //运行键盘处理进程
    RunProcess(&hd_process,hd_server,0); //测试进程能否返回成功！
    Sector s;
    ReadHardDiskMes(0);
    for(int i =0;i<512;i++){
        s.data[i] = 0xff;
    }
    WriteHDLinear(0x3000,&s,200);
    MakeFileSystem(parts[2]); //制作文件系统
    do_open("C:/././././",1);
    while (1){
        show_str_format(0,13,"the kernel task is running :%d",number++);
        sleep(1000);
    }
}