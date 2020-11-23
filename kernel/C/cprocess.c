#include "process.h"
#include "protect.h"
#include "util.h"
#include "const.h"
#include "cmemory.h"
#include "page.h"
#define PAGDRR(x) ((u32)(x))&0xfffff000
PCB process_tab;
extern u32* page_catalog;
void initProcesTab(){
    process_tab.head=0;
    process_tab.p_size=0;
    process_tab.cursor=0;
}
//添加一个进程
void AddProcess(Process *p){
    if(process_tab.head==0){
        process_tab.head = p;
        p->next = process_tab.head; 
    }else{
        Process * temp = process_tab.head;
        while (temp->next!=process_tab.head)
            temp = temp->next;
        temp->next = p;
        p->next = process_tab.head;
    }
    process_tab.p_size++;
}
Process * fetch(){
    if(process_tab.cursor==0){
        Process * res = process_tab.head;
        process_tab.cursor = res;
        return res;
    }else{
        Process * res;    
        process_tab.cursor = process_tab.cursor->next;
        res = process_tab.cursor;
        show_str_format(0,0,"%d process are running.....",res->PID);
        return res;
    }
}
//初始化进程
void  InitProcess(Process * p,void * enter){
    //初始,0,化寄存器
    p->EAX = 0;
    p->EBX = 0;
    p->ECX = 0;
    p->EDX = 0;
    p->EDI = 0;
    p->ESI = 0;
    p->EIP = enter;
    p->ESP = 0x80005000;
    p->EFLAGE =0x00000206;
    p->CS = 0x0008;
    //初始化LDT
    //p->ldt.ldt_len = 0xffff;
    //p->ldt.ldt_high_addr=0x0008;
    //p->ldt.ldt_low_addr = 0x0000;
    //创建LDT描述符
   // Descriptor desc = make_descriptor(u16_and_u16(p->ldt.ldt_high_addr,p->ldt.ldt_low_addr),48,UNIT_1BY|P_OK|DPL_3|S_CODE|TYPE_LDT);
    //p->ldt_sel = SetUpGdtDescriptor(desc);    
    //Descriptor DES_4GB_DATA = make_descriptor(0,0xfffff,UNIT_4KB|DB32|P_OK|DPL_0|S_DATA|TYPE_DATA_RW);
    //u32 selc_4GB = SetUpLdtDescriptor(&p->ldt,DES_4GB_DATA,p->ldt_sel);
    p->PID = process_tab.p_size+1; //设置PID
    p->page_catalog = malloc(4096); //为页目录开辟空间，放在系统内存中
    memcpy(p->page_catalog,page_catalog,4096/2); //将内核页目录的前一半拷贝以下
    page_catalog = p->page_catalog; //更换页目录
    page_catalog[1023] = PAGDRR(page_catalog) | PAGE_TABLE_ATTR;
    *(page_catalog+1024) = *(page_catalog+1024);
    //加载页目录，并且初始化堆栈空间
    loda_page_catelog(page_catalog);
    for(int i = 0x80000000;i<0x80005000;i=i+0x1000){
        AddPageOnadd((u32)i);
    }
    //添加进程到表中
    AddProcess(p);
}
//初始TSS
void initTSS(TSS * tss){

}
//得到当前运行的进程
Process * getCurrentP(){
    return process_tab.cursor;
}