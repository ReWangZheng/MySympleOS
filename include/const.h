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

#define PAGE_CATALOG_ATTR 0x03
#define PAGE_TABLE_ATTR 0x07
#define PAGE__ATTR 0x07

//描述符定义

//粒度-->G
#define UNIT_4KB 0x8000
#define UNIT_1BY 0x0000

//操作位-->D/B
#define DB32 0x0400
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

#define TYPE_LDT 0x0002


#endif
