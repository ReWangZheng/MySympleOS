#ifndef FILESYSTEM
#define FILESYSTEM
#include "harddisk.h"

#define Extended 0x05
#define SYMPLE_OS_IDENT 0x99
#define MAX_INODE_COUNT 4096
#define LBA_to_ADDR(lba) (lba)*512
/*
文件属性:attr 16 位
0 0 0 0  |   0 0 0 0   |  0 0 0 0  |  0 0 0 0 
                                      文件属性
                                      1：目录
                                      2：可打开文件
                                      3：可执行文件

*/

#define ATTR_DIR 0x0001
#define ATTR_TXT 0x0002

#include "type.h"
struct superblock{
    u32 fs_type; // 文件系统类型
    u32 maxcount_inode; // 最大inode数量
    u32 inode_array_sec_count; //inode-array 所占的扇区数
    u32 inode_map_sec_count; //inode-map 所占的扇区数
    u32 sec_map_count; //数据map所占的扇区数 
    u32 root_inode_id; //根目录的inode编号
    u32 inode_array_start; //inode-array的开始扇区
    u32 inode_map_start; //inode-array-map的开始扇区
    u32 sec_map_start; // sec-map的开始扇区
    u32 first_data_sec; // 第一个数据区的扇区数
};

// 2 + 2 + 16 + 32 + 4 = 40个字节 
typedef struct inode{
    u16 id; //inode唯一标识符 2个字节
    u16 attr; // 属性 2个字节
    char name[16]; //文件名字 --->16个字节
    u32 data_list[8]; // 数据所在扇区 16字节
    u32 file_size; //4个字节 文件大小
}inode;

// 18个字节
struct dir_item{
    char name[16]; //文件名字 16个字节
    u16 id; //inode-id  2个字节
}dir_item;

struct file_desc{
    int fd_mode;
    int fd_pos;
    struct inode * fd_inode;
}file_desc;

void MakeFileSystem(PartInfor info);
int do_open(char * filename,int mode);
void do_mkfile(char * dir,char * file_name,u16 attr);
#endif