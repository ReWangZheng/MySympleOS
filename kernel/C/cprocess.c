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
void  InitProcess(Process * p,void * enter,u32 DPL){
    /*下面开始初始化进程信息*/
    p->EAX = 0;
    p->EBX = 0;
    p->ECX = 0;
    p->EDX = 0;
    p->EDI = 0;
    p->ESI = 0;
    p->EIP = enter;
    p->EFLAGE =0x00000206;
    p->CS = 0x0008;
    p->ds = 0x0010;
    p->es = 0x0010;
    p->fs = 0x0000;
    p->gs = 0x0000;
    //设置进程的特权级
    p->DPL = DPL;
    //设置PID
    p->PID = process_tab.p_size+1; 

    /*下面开始配置页目录*/
    //为页目录开辟空间，放在系统内存中
    p->page_catalog = malloc(4096); 
    //将内核页目录的前一半拷贝给新页表
    memcpy(p->page_catalog,page_catalog,4096/2); 
     //更换页目录
    page_catalog = p->page_catalog;
    //设在页目录的最后一项
    page_catalog[1023] = PAGDRR(page_catalog) | PAGE_TABLE_ATTR;
    *(page_catalog+1024) = *(page_catalog+1024);
    //加载页目录，并且初始化堆栈空间
    loda_page_catelog(page_catalog);
    //初始化堆栈空间
    for(int i = 0x80000000;i<0x80020000;i=i+0x1000){
        AddPageOnadd((u32)i);
    }
    /**********************/
    /*下面开始配置LDT*/
    //为ldt分配内存空间(全局的)
    p->ldt =(LDT*)malloc(6);
    //ldt描述符装配的内存空间(局部的)，从0x80020000开始
    p->ldt->ldt_low_addr = 0x0000;
    p->ldt->ldt_high_addr = 0x8002;
    //6个描述符，6*8-1=47 
    p->ldt->ldt_len = -1;
    /*******************/

    /*下面开始配置TSS*/
    //先分配一段内存空间，标准大小
    p->tss = (TSS*)malloc(104);
    p->tss->IOOP = 103; //无IO许可位置
    //制作DPL0的堆栈空间
    Descriptor ss0_des = make_descriptor(0,0x80000000,UNIT_1BY|DB32|P_OK|DPL|S_DATA|TYPE_DATA_RW_D);
    p->tss->ESP0 = 0x80004fff;
    p->tss->SS0_sel =  SetUpLdtDescriptor(p->ldt,ss0_des);
    //制作DPL1的堆栈空间
    Descriptor ss1_des = make_descriptor(0,0x80005000,UNIT_1BY|DB32|P_OK|DPL|S_DATA|TYPE_DATA_RW_D);
    p->tss->ESP1 = 0x80009fff;
    p->tss->SS1_sel =  SetUpLdtDescriptor(p->ldt,ss1_des);
    //制作DPL2的堆栈空间
    p->tss->ESP2 = 0x8000Efff;
    Descriptor ss2_des = make_descriptor(0,0x800a000,UNIT_1BY|DB32|P_OK|DPL|S_DATA|TYPE_DATA_RW_D);
    p->tss->SS2_sel =  SetUpLdtDescriptor(p->ldt,ss2_des);
    //制作DPL3的堆栈空间
    u32 ESP3 = 0x80013FFF;
    Descriptor ss3_des = make_descriptor(0,0x8000f000,UNIT_1BY|DB32|P_OK|DPL|S_DATA|TYPE_DATA_RW_D);
    u32 SS3_sel =  SetUpLdtDescriptor(p->ldt,ss3_des);
        switch (DPL)
    {
    case DPL_0:
        p->ESP = p->tss->ESP0;
        p->ss = p->tss->SS0_sel;
        break;
    case DPL_1:
        p->ESP = p->tss->ESP1;
        p->ss = p->tss->SS1_sel;
        break;
    case DPL_2:
        p->ESP = p->tss->ESP2;
        p->ss = p->tss->SS2_sel;
        break;
    case DPL_3:
        p->ESP = ESP3;
        p->ss = SS3_sel;
        break;
    }
    //装载LDT到GDT中
    Descriptor ldt_des = make_descriptor(0x80020000,p->ldt->ldt_len,UNIT_1BY|P_OK|p->DPL|TYPE_LDT);
    p->ldt_sel = SetUpGdtDescriptor(ldt_des);
    
    //制作TSS描述符，并且将其安装到GDT中
    Descriptor tss_des = make_descriptor(p->tss,103,UNIT_1BY|P_OK|DPL|TYPE_TSS);
    p->tss_sel = SetUpGdtDescriptor(tss_des); 
    /***************/
    AddProcess(p);
}
//初始TSS
void initTSS(TSS * tss){

}
//得到当前运行的进程
Process * getCurrentP(){
    return process_tab.cursor;
}