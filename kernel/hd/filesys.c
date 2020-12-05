#include "summary.h"
extern PartInfor parts[10];
//对某分区制作文件系统 ,参数：分区表项
void MakeFileSystem(PartInfor info){
    if(info.Type == 0x99){
        SimpleFS(info);
    }
}

// 制作简单文件系统
Sector temp[20]; // 缓冲区
void SimpleFS(PartInfor info){
    // 初始化缓存区
    for(int j =0;j<5;j++){
        for(int i =0;i<512;i++){
            temp[j].data[i] = 0;
        }
    } 
    /*
        1 sector        320 sectors           1 sector         5         start:327
    super-block ---> inode-array[4096] ----> inode-map ----> sec-map---->data
    */
    
    struct superblock sb; // 创建一个数据块的数据结构
    sb.fs_type = SYMPLE_OS_IDENT; //文件系统标识符
    sb.maxcount_inode = MAX_INODE_COUNT; //最大inode数量
    sb.inode_array_sec_count = sizeof(struct inode) * MAX_INODE_COUNT / 512;
    sb.inode_map_sec_count = MAX_INODE_COUNT / (512 * 8);
    sb.sec_map_count = info.SEC_COUNT%512==0 ? info.SEC_COUNT / (512 * 8):info.SEC_COUNT / (512 * 8)+1;
    sb.root_inode_id = 0;
    sb.inode_array_start = info.LBA_START + 1;
    sb.inode_map_start = sb.inode_array_start + sb.inode_array_sec_count;
    sb.sec_map_start = sb.inode_map_start + sb.inode_map_sec_count;
    sb.first_data_sec = info.LBA_START + 1 +sb.inode_array_sec_count + sb.inode_map_sec_count + sb.sec_map_count; // 这是数据的起始扇区
    
    //下面开始写入super block块
    WriteHDLinear(LBA_to_ADDR(info.LBA_START),&sb,sizeof(struct superblock));    

    //下面先处理inode位图
    temp[0].data[0] = 1;
    WriteHDLBA(sb.inode_array_start,temp,1);
    // 下面处理扇区位图 --> 前面327个扇区都是已经被使用了的，200位 / 8 = 25个字节 均为1
    int use_sec_size = (1+sb.inode_array_sec_count+sb.inode_map_sec_count+sb.sec_map_count);
    int bytes = use_sec_size%8==0 ? use_sec_size/8 : use_sec_size/8+1;

    for(int i =0;i< bytes;i++){
        temp[0].data[i] = 0xff;
    }
//    写扇区
    WriteHDLBA(sb.sec_map_start,temp,1);
    // 下面创建根目录的inode
    struct inode root;
    root.id = 0;
    char rootname[] = "root";
    strcp(root.name,rootname);
    
    root.attr = ATTR_DIR;
    for(int i =0;i<8;i++){
        root.data_list[i] = 0;
    }
    // 下面寻找一个空闲扇区
    ReadHDLBA(sb.sec_map_start,temp,5);
    int sec_index = 0;
    int byte_index = 0;
    int bit_index = -1;
    // 下面开始搜索
    for(int j = 0;j < 5;j++){
        for(int i = 0;i<512;i++){

            bit_index=bit_test(temp[j].data[i],0);
            if(bit_index!=-1){

                sec_index = j;
                byte_index = i;
                break;
            }
        }
        if(bit_index!=-1){
            break;
        }
    } 
    // 将该标志设置为1
    temp[sec_index].data[byte_index] |=(1<<bit_index); 
    WriteHDLBA(sb.sec_map_start,&temp[sec_index],1);
    //开始写入数据 (根目录初始时候为0)    
    struct dir_item item;
    item.id = 0;
    *(item.name) = ".";
    // 我们得到的空闲扇区
    int unuse_sec = (sec_index * 4096) + byte_index * 8 +bit_index; 
    root.data_list[0] = unuse_sec;
    // 将根目录信息写入到扇区中
    
    WriteHDLinear((info.LBA_START + unuse_sec) * 512,&item,sizeof(struct dir_item));
    // 将这个根目录inode写入扇区中
    root.file_size = sizeof(struct dir_item); // 修改文件大小
    WriteHDLinear(LBA_to_ADDR(sb.inode_array_start),&root,sizeof(struct inode));
}

//得到分区信息
PartInfor* getinfo(char * name){
    for(int i =0;i<10;i++){
        if(strcmp(parts[i].Name,name)==0){
            return &parts[i];
        }
    }
}
void getsuperblock(struct superblock *s,PartInfor *p){
    int lba_s = p->LBA_START;
    ReadHDLinear(lba_s * 512,s,sizeof(struct superblock));
}

inode getinode(char * filename){
    
}


// 方法：创建文件夹
void do_mkdir(char * path){
    

}

//方法：打开文件
#define INODE_TEMP_SIZE 1
inode nodes_temp[INODE_TEMP_SIZE];

int do_open(char * filename,int mode){
    char part[2];
    part[0] = filename[0];
    PartInfor *part_mes = getinfo(part);
    //申请一个super block
    struct superblock s;
    //得到了super block
    getsuperblock(&s,part_mes);
    //根目录的id    
    u32 rooid = s.root_inode_id;
    //16个inode缓冲
    inode *root_inode;
    int max_cicle = s.inode_array_sec_count / INODE_TEMP_SIZE;
    u32 addr_temp = s.inode_array_start * 512;
}


