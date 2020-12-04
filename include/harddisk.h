#ifndef HARDDISK
#define HARDDISK

#define PRIMARY_DATA_PORT 0x1f0
#define PRIMARY_ERR_PORT 0x1f1
#define PRIMARY_SECS_PORT 0x1f2
#define PRIMARY_LBA_L_PORT 0x1f3
#define PRIMARY_LBA_M_PORT 0x1f4
#define PRIMARY_LBA_H_PORT 0x1f5

#define PRIMARY_DEVICE_PORT 0x1f6

#define PRIMARY_STATUS_PORT 0x1f7
#define PRIMARY_COMMAND_PORT 0x1f7

#define PRIMARY_CONTOL_PORT 0x3f6

#define DEVICE_PRIMARY_LBA_MODE 0xe0

#define COMMAND_REQUEST_READ 0x20
#define COMMAND_REQUEST_WRITE 0x30
#include "type.h"
typedef struct HardDiskMes
{
    char id[21];
    int hd_count;
    int hd_selector;
}HardDiskMes;


typedef struct PartInfor 
{
    /* data */
    char Name[10];
    u32 ID;
    u32 State;
    u32 Type;
    u32 LBA_START;
    u32 SEC_COUNT;
}PartInfor;



typedef  struct Sector
{
    u8 data[512];
}Sector;

void hd_server();
void hd_handle();
void ReadHardDiskMes(int HD); //根据设备号读取分区信息
void ReadHDLBA(u32 sec_start,Sector * buf,int size); //LBA读取扇区
void ReadHDLinear(u32 start,u8 * buf,int len);//根据线性地址读取硬盘
void WriteHDLBA(u32 sec_start,Sector * buf,int size);
void WriteHDLinear(u32 start,u8 * buf,int len);

#endif

