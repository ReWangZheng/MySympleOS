#include "harddisk.h"
#include "type.h"
#include "util.h"
#include "process.h"
#include "ctime.h"
//硬盘中断服务程序
void hd_handle(){
    show_str_format(0,4,"hard-dist!");
}
//硬盘驱动服务程序
void hd_server(){
    // HardDiskMes mes;
    // getHdiskMes(&mes);
    int number = 0;
    while (1)
    {
        sleep(1000);
        show_str_format(0,3,"hd server:%d",number++);
        //Process *p = getCurrentP();
    }
}