#ifndef INTERRUPT
#define INTERRUPT
#include "cInterrupt.h"
void Interrupt_0();//时钟周期中断
void keybordInt(); //键盘中断
void close_int(); // 关中断
void open_int(); //开中断
void DE_ERR();
void TS_ERR();
void NP_ERR();
void SS_ERR();
void GP_ERR();
void PF_ERR();
#endif
