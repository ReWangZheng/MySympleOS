#include "cerror.h"


void panic(char *format){
    show_str_format(0,10,format);
    ud2_indicate();
}

void assertion_failure(char *exp,char *file,char * base_file,int line){
    clean_screen();
    show_str_format(0,0,"expresion:%s  ,file:%s  ,base_file:%s  ,line:%d",exp,file,base_file,line);
}