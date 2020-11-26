#include "process.h"
#include "util.h"
#include "ctime.h"
#include "keybord.h"
#include "const.h"
#include "page.h"
#include "Interrupt.h"
Process keybod_process;
int __kernel__(){
    show_str_format(0,15,"the kernel task start running ");

    InitKeyBuf();
    
    int number = 0;
    while (1){
        show_str_format(0,15,"the kernel task is running : %x",number++);
    }
}