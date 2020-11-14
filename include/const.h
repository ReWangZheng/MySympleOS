#ifndef CONST
#define CONST
#include "type.h"
#define MASTER_8259_PORT1 0x20
#define MASTER_8259_PORT2 0x21

#define CASCADE_8259_PORT1 0xa0
#define CASCADE_8259_PORT2 0xa1

#define ICW1 0x11
#define ICW2_MAST 0x20 //主片中断向量从0x20开始
#define ICW2_CASCADE 0x28 //从片从0x28开始
#define ICW3_MAST 0x04 //IR2 对应的是从片
#define ICW3_CASCADE 0x02 //IR2 对应的是从片

#define ICW4 0x01 //

#define OCW1_MAST 0Xfe // 仅开启时钟中断
#define OCW1_CASCADE 0xff //关闭从片所有中断


#define IDT_LEN 0x140
#define IDT_ADDR 0x00003000

#define RTC_0X70 0x70
#define RTC_0X74 0x74

#define EOI 0x20

#define Sele_Code_4GB 0x08

#endif