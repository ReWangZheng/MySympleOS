#ifndef HARDDISK
#define HARDDISK

#define PRIMARY_DATA_PORT 0x1f0
#define PRIMARY_ERR_PORT 0x1f1
#define PRIMARY_SECS_PORT 0x1f2
#define PRIMARY_LBA_L_PORT 0x1f3
#define PRIMARY_LBA_M_PORT 0x1f4
#define PRIMARY_LBA_H_PORT 0x1f5

#define PRIMARY_DEVICE_PORT 0x1f6

#define PRIMARY_STATUS_PORT 0x1f6
#define PRIMARY_COMMAND_PORT 0x1f6

#define PRIMARY_CONTOL_PORT 0x3f6

#define DEVICE_PRIMARY_LBA_MODE 0xe0

typedef struct HardDiskMes
{
    char id[21];
    int hd_count;
    int hd_selector;
}HardDiskMes;


void hd_server();
void hd_handle();
#endif

