#include "harddisk.h"
#include "type.h"
#include "util.h"
#include "process.h"
#include "ctime.h"
#include "cmemory.h"
#include "filesys.h"

//硬盘中断服务程序
void hd_handle(){
    show_str_format(0,4,"hard-dist!");
}
//硬盘驱动服务程序

// inode 缓冲区

void hd_server(){
    int number = 0;
    while (1)
    {
        sleep(1000);
        show_str_format(0,3,"hd server:%d",number++);
    }
}
// 读取扇区
void ReadHDLBA(u32 sec_start,Sector * buf,int size){
    //第一步，将读取数量写入端口0x1f2
    out_byte(PRIMARY_SECS_PORT,size);
    //第二步，设置扇区号
    out_byte(PRIMARY_LBA_L_PORT,(sec_start>>0)&0xff);
    out_byte(PRIMARY_LBA_M_PORT,(sec_start>>8)&0xff);
    out_byte(PRIMARY_LBA_H_PORT,(sec_start>>16)&0xff);
    out_byte(PRIMARY_DEVICE_PORT,((sec_start>>24)&0x0f)|DEVICE_PRIMARY_LBA_MODE);
    //第三步，请求读
    out_byte(PRIMARY_COMMAND_PORT,COMMAND_REQUEST_READ);
    //第四步，查看状态
    u8 state = in_byte(PRIMARY_STATUS_PORT);
    // 等待硬盘完成
    int nub = 0;
    while ((state&0x80)==0x80){
        state = in_byte(PRIMARY_STATUS_PORT);
    }
    //开始读数据
    for(int i = 0;i < size;i++){
        for(int j =0;j<512;j+=2){
            u16 word = in_byte_16(PRIMARY_DATA_PORT); //读取两个字节
            buf[i].data[j] =  (u8)word;
            buf[i].data[j+1] = (u8)(word>>8); //向右边移动8个字节
        }
    }
}
// 写扇区
void WriteHDLBA(u32 sec_start,Sector * buf,int size){
    //第一步，将读取数量写入端口0x1f2
    out_byte(PRIMARY_SECS_PORT,size);
    //第二步，设置扇区号
    out_byte(PRIMARY_LBA_L_PORT,(sec_start>>0)&0xff);
    out_byte(PRIMARY_LBA_M_PORT,(sec_start>>8)&0xff);
    out_byte(PRIMARY_LBA_H_PORT,(sec_start>>16)&0xff);
    out_byte(PRIMARY_DEVICE_PORT,((sec_start>>24)&0x0f)|DEVICE_PRIMARY_LBA_MODE);
    //第三步，请求读
    out_byte(PRIMARY_COMMAND_PORT,COMMAND_REQUEST_WRITE);
    //第四步，查看状态
    u8 state = in_byte(PRIMARY_STATUS_PORT);
    // 等待硬盘完成
    int nub = 0;
    while ((state&0x80)==0x80){
        state = in_byte(PRIMARY_STATUS_PORT);
    }
    //开始写数据
    for(int i = 0;i < size;i++){
        for(int j =0;j<512;j+=2){
            out_byte_16(PRIMARY_DATA_PORT,u8_and_u8(buf[i].data[j+1],buf[i].data[j]));
        }
    }
}

// 根据数据在磁盘中的线性地址来读取
void ReadHDLinear(u32 start,u8 * buf,int len){
    // 线性地址在扇区的起始点
    int sector_start = start / 512;
    // 线性地址在扇区中的偏移
    int offset = start % 512;
    // 读取扇区的数量
    int sector_count =  (len % 512 == 0) ? len/512 : len/512+1;
    // 要读取扇区的的多少
    Sector sec[sector_count];
    //开始读取扇区
    ReadHDLBA(sector_start,sec,sector_count);
    for(int i =0;i<len;i++){
        int sec_index = i / 512;
        int sec_offset =  (offset+i) % 512;
        *(buf+i) = sec[sec_index].data[sec_offset];
    }
}
// 根据数据在磁盘中的线性地址来写
void WriteHDLinear(u32 start,u8 * buf,int len){
    // 线性地址在扇区的起始点
    int sector_start = start / 512;
    // 线性地址在扇区中的偏移
    int offset = start % 512;
    //先将那一整个扇区读取出来
    Sector first_sec;
    // 即将写扇区的数量
    int sector_count =  (len % 512 == 0) ? len/512 : len/512+1;
    // 要读取扇区的的多少
    Sector sec[sector_count];
    // 先把原来的数据读出来
    ReadHDLBA(sector_start,sec,sector_count);
    for(int i =0;i<len;i++){
        int sec_index = i / 512;
        int sec_offset =  (offset+i) % 512;
        sec[sec_index].data[sec_offset]= *(buf+i);
    }
    // 然后我们在覆盖原来的数据
    WriteHDLBA(sector_start,sec,sector_count);
}
// 获取硬盘分区表信息
PartInfor parts[10];
void ReadHardDiskMes(int HD){
    // 申请一块缓存区域
    u8 * buf =(u8*)malloc(32);
    // 先读取四个主区分
    u32 addr = 0x1be;
    // 分区名字
    char name[10]={'A','B','C','D','E','F','G','H','I','J'};
    // 是否有逻辑分区标记
    int logist_part = -1;
    // 循环变量,后面还会用到,所以先留下
    int i;
    for(i =0;i<4;i++){
        //根据线性地址读硬盘
        ReadHDLinear(addr,buf,16);
        // 先申明一个分区表项
        PartInfor info;
        info.State = *(buf+0);
        info.Type = *(buf+4);
        info.LBA_START = *((u32*)(buf+8));
        info.SEC_COUNT = *((u32*)(buf+12));
        //判断该分区字段是否有效
        if(info.SEC_COUNT==0){
            break;
        }
        parts[i] = info;
        parts[i].Name[0] = name[i];
        parts[i].Name[1] = 0;
        parts[i].ID = i;
        if(parts[i].Type == Extended){
            logist_part = i;
        }
        show_str_format(0,14+i,"Name:%s State %x,Type %x,Start %x,Count %x",parts[i].Name,parts[i].State,parts[i].Type,parts[i].LBA_START,parts[i].SEC_COUNT);
        addr +=16;
    }
    //如果包含有逻辑分区
    if(logist_part!=-1){
        //先读一下扩展分区
        PartInfor logistp = parts[logist_part];
        // 计算出逻辑分区的地址
        u32 addr = logistp.LBA_START * 512+0x1be;
        int start_sec = logistp.LBA_START;
        for(;i<10;i++){
            // 读取分区表项
            ReadHDLinear(addr,buf,32);
            PartInfor info;
            info.State = *(buf+0);
            info.Type = *(buf+4);
            info.LBA_START = *((u32*)(buf+8)) + start_sec;
            info.SEC_COUNT = *((u32*)(buf+12));
            
            if(info.SEC_COUNT==0){
                  break;
            }
            parts[i] = info;
            parts[i].Name[0] = name[i];
            parts[i].Name[1] = 0;
            parts[i].ID = i;
            show_str_format(0,14+i,"extend Name:%s State %x,Type %x,Start %x,Count %x",parts[i].Name,parts[i].State,parts[i].Type,parts[i].LBA_START,parts[i].SEC_COUNT);
            //判断是否还有逻辑分区
            if( *(buf+16+4) != Extended){
                break;
            }else{
                // 下一个分区的偏移位置
                addr = (*((u32*)(buf+16+8))+logistp.LBA_START) * 512 + 0x1be;
                start_sec = (*((u32*)(buf+16+8))+logistp.LBA_START);
            }
        }
    }
}

