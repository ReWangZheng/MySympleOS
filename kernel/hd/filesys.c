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
    strcp(root.name,"root");
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
    strcp(item.name,".");
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

#define INODE_TEMP_SIZE 16
inode nodes_temp[INODE_TEMP_SIZE];

// 通过inode 得到 文件列表
static struct dir_item * getDirItem(inode *node,PartInfor *info,int *res){
    //当前目录的item数量
    int item_size = node->file_size / sizeof(struct dir_item);
    *res = item_size;
    // 一个扇区最大的item数量
    int sec_max_item_size = 512 / sizeof(struct dir_item);
    
    struct dir_item *items= malloc(sizeof(struct dir_item) * item_size);
    
    int k = node->file_size % 512==0?node->file_size/512:(node->file_size/512)+1;
    for(int i = 0;i <k;i++){
        u32 ds = node->data_list[i];
        u32 data_sector = info->LBA_START + ds;
        int len = 0;
        if(sec_max_item_size < item_size){
            len = sec_max_item_size * sizeof(struct dir_item);
            ReadHDLinear(LBA_to_ADDR(data_sector),items,len);
            item_size-=sec_max_item_size;
            items+=sec_max_item_size;
        }else{
            len = item_size * sizeof(struct dir_item);
            ReadHDLinear(LBA_to_ADDR(data_sector),items,len);
            break;
        }
    }
    return items;
}
//通过inode—id 得到 inode
static inode *getInodeByid(struct superblock * s,int id){
    //16个inode缓冲
    inode *res=malloc(sizeof(inode));
    // 计算inode的起始线性地址
    u32 addr_temp = s->inode_array_start * 512;
    // 创建一个inode位图的buf
    Sector inode_map_buf[s->inode_map_sec_count]; 
    //得到inode位图
    getInodemap(s,inode_map_buf);
    //将扇区型数据转为字节形数据
    u8 * buf = (u8*)inode_map_buf;
    int cursor=0;
    for(int i =0;i<s->inode_array_sec_count / INODE_TEMP_SIZE;i++){
        //读取到缓冲区
        ReadHDLinear(addr_temp + sizeof(inode) * INODE_TEMP_SIZE * i,nodes_temp,sizeof(inode));
        // 开始对缓冲区中的inode进行匹配
        for(int j=0;j<INODE_TEMP_SIZE;j++){
            //如果显示当前位图还没有被分配，则说明以及全部查找完毕，且没有找到
            if(bit(buf,cursor++)==0){
                addr_temp = 0;
                return NULL;
            }
            // 如果匹配到了相应的inode，则返回结果
            if(nodes_temp[j].id==id){
                memcpy(res,&nodes_temp[j],sizeof(inode));
                return res;
            }
        }
    }
    return res;
}
//得到分区的inode位图
static void  getInodemap(struct superblock *s,Sector * buf){
    int start = s->inode_map_start;
    int sec_count = s->inode_map_sec_count;
    ReadHDLBA(start,buf,sec_count);
}
//给定文件路径，递归搜索文件
static inode * search_file_inode(struct superblock *s,PartInfor *info,char path[10][16],int deep_cur,int max_deep,inode * node){
    if(deep_cur==max_deep){
        return node;
    }
    if((node->attr&ATTR_DIR) != ATTR_DIR){
        return NULL;
    }
    int item_size = 0;
    struct dir_item *items = getDirItem(node,info,&item_size);
    inode * target = NULL;
    // 遍历目录，寻找文件
    for(int j = 0;j<item_size;j++){
        // 判断该项是否与当前路径名符合
        if(strcmp(items[j].name,path[deep_cur])==0){
            //如果符合，则根据ID取到inode
            inode* temp = getInodeByid(s,items[j].id);
            target = temp;
            break;
        }
    }
    if(target==NULL){
        return NULL; 
    }else{
        return search_file_inode(s,info,path,deep_cur+1,max_deep,target);
    }
}
// 通过路径名获取inode
inode * getinode(char * filename){
    // 路径名字分割数组； 最长10个 每个最大为16
    char path_split[10][16]={0};
    // 得到分割数组 以及 路径长度：最小为1
    int path_deep = split(filename,'/',path_split,10);

    if(path_deep>=10){
        return NULL;
    }
    path_split[0][1]=0;
    if(path_split[path_deep-1][0]==0){
        strcp(path_split[path_deep-1],".");
    }
    // 根据路径中的分区,得到分区信息
    PartInfor *part_mes = getinfo(path_split[0]);
    //申请一个super block
    struct superblock s;
    //得到了super block
    getsuperblock(&s,part_mes);
    //根目录的id    
    u32 rooid = s.root_inode_id;
    // 得到指定分区的root节点
    inode *root = getInodeByid(&s,rooid);
    // 如果想到得到的就root目录，则直接返回
    if(path_deep==1){
        return root;
    }

    // 缓冲指针
    inode *temp = root;
    //得到根目录的文件信息
    int item_size=0;  //目录条数
    return search_file_inode(&s,part_mes,path_split,1,path_deep,root);
}

// 方法：创建文件夹
void do_mkdir(char * path){

}


//方法：创建文件
void do_mkfile(char * dir,char * file_name,u16 attr){
    //先得到目标目录的inode
    inode *dir_inode = getinode(dir);
    // 如果给定目录不存在，或则不是目录
    if(dir_inode==NULL||dir_inode->attr & ATTR_DIR !=ATTR_DIR){
        return;
    }
}

void do_write(inode * inode,u8 *buf,int len){
    // 得到文件大小
    int fs = inode->file_size;
    // 得到字节在扇区中的偏移
    int byte_offset = fs % 512;
    // 得到我们应该写的起始扇区
    int list_offset = fs / 512 + 1;
    // 如果刚刚是一个扇区的化
    if(inode->data_list[list_offset]==0){
        //分配一个扇区
    }else{
        //得到想要写的扇区
        int start = inode->data_list[list_offset];
        //往扇区里写数据 :注意，这里写的数据 byte_offset+len 应该小于等于512
        WriteHDLinear(start*512+byte_offset,buf,len);

    }   
    
}

//为给定inode分配指定数量的扇区
void allocate_sector(inode * inode,int size){

}

//分配一个inode
inode* allocate_inode(char * file_name,u32 file_size,u16 file_attr){
    inode * res;
    
    return res;
}

//方法：打开文件
int do_open(char * filename,int mode){
    inode *i = getinode(filename);
    if(i!=NULL){
        show_str_format(0,6,"items:%d",i->file_size);
    }else
    {
        show_str_format(0,6,"no items");
    }
}


