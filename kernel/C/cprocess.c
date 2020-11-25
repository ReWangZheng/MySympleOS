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
    process_tab.tss = 0;
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
    p->CS = 0x08;
    p->EIP = enter;
    p->EFLAGE =0x00000206;
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
    p->page_catalog = malloc_4096_align();
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
    //制作DPL0的堆栈空间
    Descriptor ss0_des = make_descriptor(0,(u32)0x80000000,UNIT_1BY|DB32|P_OK|DPL_0|S_DATA|TYPE_DATA_RW_D);
    u32  ESP0 = 0x80004fff;
    u32  SS0_sel =  SetUpLdtDescriptor(p->ldt,ss0_des);
    //制作DPL1的堆栈空间
    Descriptor ss1_des = make_descriptor(0,(u32)0x80005000,UNIT_1BY|DB32|P_OK|DPL_1|S_DATA|TYPE_DATA_RW_D);
    u32  ESP1 = 0x80009fff;
    u32  SS1_sel =  SetUpLdtDescriptor(p->ldt,ss1_des);
    //制作DPL2的堆栈空间
    u32  ESP2 = 0x8000Efff;
    Descriptor ss2_des = make_descriptor(0,(u32)0x800a000,UNIT_1BY|DB32|P_OK|DPL_2|S_DATA|TYPE_DATA_RW_D);
    show_str_format(0,8,"ss2:%x %x",ss2_des.high>>16,ss2_des.low);
    u32  SS2_sel =  SetUpLdtDescriptor(p->ldt,ss2_des);
    //制作DPL3的堆栈空间
    u32 ESP3 = 0x80013FFF;
    Descriptor ss3_des = make_descriptor(0,(u32)0x8000f000,UNIT_1BY|DB32|P_OK|DPL_3|S_DATA|TYPE_DATA_RW_D);
    u32 SS3_sel =  SetUpLdtDescriptor(p->ldt,ss3_des);
    initTSS(SS0_sel,ESP0,SS1_sel,ESP1,SS2_sel,ESP2);
        switch (DPL)
    {
    case DPL_0:
        p->ESP = ESP0;
        p->ss = SS0_sel|0;
        break;
    case DPL_1:
        p->ESP = ESP1;
        p->ss = SS1_sel|1;
        break;
    case DPL_2:
        p->ESP = ESP2;
        p->ss = SS2_sel|2;
        break;
    case DPL_3:
        p->ESP = ESP3;
        p->ss = SS3_sel|3;
        break;
    }
    //装载LDT到GDT中
    Descriptor ldt_des = make_descriptor(0x80020000,p->ldt->ldt_len,UNIT_1BY|P_OK|p->DPL|TYPE_LDT);
    p->ldt_sel = SetUpGdtDescriptor(ldt_des);
    /***************/
    AddProcess(p);
}
//初始TSS
void initTSS(u32 ss0,u32 esp0,u32 ss1,u32 esp1,u32 ss2,u32 esp2){
    /*下面开始配置TSS*/
    //先分配一段内存空间，标准大小
    if(process_tab.tss==0){
        process_tab.tss= (TSS*)malloc(104);
        process_tab.tss->IOOP = 103; //无IO许可位置
        process_tab.tss->SS0_sel =ss0;
        process_tab.tss->SS1_sel = ss1;
        process_tab.tss->SS2_sel = ss2;
        process_tab.tss->ESP0 = esp0;
        process_tab.tss->ESP1 = esp1;
        process_tab.tss->ESP2 = esp2;
        //制作TSS描述符，并且将其安装到GDT中
        Descriptor tss_des = make_descriptor(process_tab.tss,103,UNIT_1BY|P_OK|DPL_0|TYPE_TSS);
        u32 tss_sel = SetUpGdtDescriptor(tss_des); 
        LoadTSS(tss_sel);
    }
}
//得到当前运行的进程
Process * getCurrentP(){
    return process_tab.cursor;
}