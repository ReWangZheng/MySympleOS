#include "process.h"
#include "util.h"
#include "ctime.h"
int __kernel__(){
    int number = 0;
    while (1){
        show_str_format(0,15,"the kernel task is running : %x",number++);
    }
}