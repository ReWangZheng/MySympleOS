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

#define OCW1_MAST 0Xfc // 仅开启时钟中断
#define OCW1_CASCADE 0xff //关闭从片所有中断


#define IDT_LEN 0x140
#define IDT_ADDR 0x00003000

#define RTC_0X70 0x70
#define RTC_0X74 0x74

#define EOI 0x20

#define Sele_Code_4GB 0x08

#define PAGE_CATALOG_ATTR 0x03
#define PAGE_TABLE_ATTR 0x07
#define PAGE__ATTR 0x07

//描述符定义

//粒度-->G
#define UNIT_4KB 0x8000
#define UNIT_1BY 0x0000

//操作位-->D/B
#define DB32 0x4000
#define DB16 0x0000

// 存在位-->P 
#define P_OK 0x0080
#define P_NO 0X0000

// 特权级-->DPL
#define DPL_0 0x0000
#define DPL_1 0x0020
#define DPL_2 0x0040
#define DPL_3 0x0060

//S
#define S_DATA 0x0010
#define S_CODE 0x0000
// 类型符-->TYPE
#define TYPE_DATA_R 0x0000
#define TYPE_DATA_RW 0x0002
#define TYPE_DATA_R_D 0x0004
#define TYPE_DATA_RW_D 0x0006

#define TYPE_TSS 0x0009

#define TYPE_LDT 0x0002

#define TYPE_CODE_X 0X0008
#define TYPE_CODE_RX 0X000a
#define TYPE_CODE_XY 0X000c
#define TYPE_CODE_RXY 0X00e

//设置定时器
#define COUNTER0_PORT 0x40
#define COUNTER1_PORT 0x41
#define COUNTER2_PORT 0x42 
#define TIMEMODE_PORT 0x43
#define COUNTER0 0x00
#define COUNTER1 0x40
#define COUNTER2 0x80
#define MODE_GENERATOR 0x04 
#define OP_LOCAK 0x00
#define OP_W_H 0x10
#define OP_W_L 0x20
#define OP_W_LH 0x30
#define UNIT_INT 16931

//键盘相关的常量
#define KEY_READ_PORT 0x60

#endif
