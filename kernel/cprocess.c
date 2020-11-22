#include "process.h"
#include "protect.h"
#include "util.h"
#include "const.h"
PCB process_tab;
void initProcesTab(){
    process_tab.head=0;
    process_tab.p_size=0;
    process_tab.cursor=0;
}
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
int number = 10;
Process * fetch(){
    if(process_tab.cursor==0){
        Process * res = process_tab.head;
        process_tab.cursor = res;
        return res;
    }else{
        Process * res;    
        process_tab.cursor = process_tab.cursor->next;
        res = process_tab.cursor;
        show_str_format(0,12,"fetch PID:%d %x",res,number++);
        return res;
    }
}
void debug(){
    u32 *esp = getESP();

    show_str_format(0,2,"eax:%x ebx:%x ecx:%x edx:%x esp:%x   ",getEAX(),getEBX(),getECX(),getEDX(),getESP());
    show_str_format(0,1,"ds:%x cs:%x ss:%x    ",getDS(),getCS(),getSS());
    show_str_format(10,0,"stack: %x %x %x",*(esp),*(esp+1),*(esp+2));

}


void  InitProcess(Process * p,void * enter){
    //初始化寄存器
    p->EAX = 0;
    p->EBX = 0;
    p->ECX = 0;
    p->EDX = 0;
    p->EDI = 0;
    p->ESI = 0;
    p->EIP = enter;
    p->ESP = 0x50000+0x3000* process_tab.p_size;
    p->EFLAGE =0x00000206;
    p->CS = 0x0008;
    //初始化LDT
    p->ldt.ldt_len = 0xffff;
    p->ldt.ldt_high_addr=0x0008;
    p->ldt.ldt_low_addr = 0x0000;
    //创建LDT描述符
   // Descriptor desc = make_descriptor(u16_and_u16(p->ldt.ldt_high_addr,p->ldt.ldt_low_addr),48,UNIT_1BY|P_OK|DPL_3|S_CODE|TYPE_LDT);
    //p->ldt_sel = SetUpGdtDescriptor(desc);    
    //Descriptor DES_4GB_DATA = make_descriptor(0,0xfffff,UNIT_4KB|DB32|P_OK|DPL_0|S_DATA|TYPE_DATA_RW);
    //u32 selc_4GB = SetUpLdtDescriptor(&p->ldt,DES_4GB_DATA,p->ldt_sel);
    p->PID = process_tab.p_size+1;
    AddProcess(p);
}

int nub1=0;
Process * getCurrentP(){
    show_str_format(4,0,"get times %d",nub1++);
    return process_tab.cursor;
}