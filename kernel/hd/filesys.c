#include "summary.h"
#include "cerror.h"
#include "cmemory.h"
extern PartInfor parts[10];
//对某分区制作文件系统 ,参数：分区表项
void MakeFileSystem(PartInfor info){
    if(info.Type == 0x99){
        SimpleFS(info);
    }
}

// 通过inode 得到 文件列表
#define INODE_TEMP_SIZE 16
inode nodes_temp[INODE_TEMP_SIZE];
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
        u32 data_sector = ds;
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

//得到分区的inode位图
static void  getInodemap(struct superblock *s,Sector * buf){
    int start = s->inode_map_start;
    int sec_count = s->inode_map_sec_count;
    ReadHDLBA(start,buf,sec_count);
}

//通过inode—id 得到 inode
static inode *getInodeByid(struct superblock * s,int id){
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
        ReadHDLinear(addr_temp + sizeof(inode) * INODE_TEMP_SIZE * i,nodes_temp,sizeof(inode) * 16);

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



//更新inode
static void update_inode(struct superblock *s,inode * n){
    u32 addr = s->inode_array_start * 512 + n->id * sizeof(inode);

    WriteHDLinear(addr,n,sizeof(inode));
}


//为给定inode分配指定数量的扇区
static int* allocate_sector(struct superblock *s,inode * node,int *in_size){

    int size = *in_size;
    int index = node->file_size %512==0?node->file_size/512:1+node->file_size/512;
    //判断扇区列表是否会越界
    if(index<7&&index+1+size>7){
        size++;
        *in_size=size;
    }
    u32 * sec_index = (u32*)malloc(size * sizeof(u32));

    //得到磁盘位图
    Sector temp[s->sec_map_count];
    ReadHDLBA(s->sec_map_start,temp,s->sec_map_count);
    //搜索空位置

    int cursor = 0; //寻找的个数
    for(int i =0;i<s->sec_map_count;i++){
        for(int j =0;j<512;j++){
            u8 byte = temp[i].data[j];
            int k = bit_test(byte,0);
         

            if(k==-1){
                continue;
            }else{
                sec_index[cursor++] = s->first_data_sec + 512*8*i + j*8 + k;
                temp[i].data[j] |= (1<<k);
                j--;
            }
            if(cursor==size){
                goto end_for;
            }
        }
    }
end_for:

    //将该空位置占用
    WriteHDLBA(s->sec_map_start,temp,s->sec_map_count);
    
    // 然后将磁盘占用信息更新到inode里
    int i=0;
    cursor=0;
    for(cursor=0,i = index;i<=7&&cursor<size;i++,cursor++){
        node->data_list[i] = sec_index[cursor];
    }
    // 如果data_list已经有被分配了7个扇区，那么我们就要在扩展map列表中进行填充

    if(i>=7&&cursor<size){

        u32 start = node->data_list[7]*512 + (i-8) * sizeof(u32);

        WriteHDLinear(start,&sec_index[cursor],(size-cursor)*sizeof(u32));
    }

    //更新inode
    update_inode(s,node);
    //返回我们得到的扇区号
    return sec_index;
}


//得到超级块
static void getsuperblock(struct superblock *s,PartInfor *p){
    int lba_s = p->LBA_START;
    ReadHDLinear(lba_s * 512,s,sizeof(struct superblock));
}

//分配一个inode
static inode* allocate_inode(struct superblock *s,char * file_name,u32 file_size,u16 file_attr){
    inode * res;
    // 第一步，我们得为这个inode分配空间
    res = (inode*)malloc(sizeof(inode));
    memset(res->data_list,0,8);

    // 第二步，得到inode-map
    Sector sec[s->inode_map_sec_count];
    getInodemap(s,sec);

    // 第三步，寻找一个空闲的bit
    for(int i =0;i<s->inode_map_sec_count;i++){
        for(int j = 0;j<512;j++){

            int index = bit_test(sec[i].data[j],0);
            if(index==-1){
                continue;
            }
            // 将该位设置为1
            sec[i].data[j]|=(1<<index);
            // 然后将将其写入硬盘
            res->id = i * 4096 + j * 8 + index;
            goto FOREND;  
        }
    }
FOREND:

    //设置文件属性
    res->attr = file_attr;
    //设置文件大小
    res->file_size = file_size;
    // 拷贝name
    strcpy(res->name,file_name);
    //更新map将其写入
    WriteHDLBA(s->inode_map_start,sec,s->inode_map_sec_count);    
    return res;
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
    WriteHDLBA(sb.inode_map_start,temp,1);
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
    strcpy(root.name,"root");
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
    strcpy(item.name,".");
    // 我们得到的空闲扇区
    int unuse_sec = (sec_index * 4096) + byte_index * 8 +bit_index; 
    root.data_list[0] = unuse_sec + info.LBA_START;
    // 将根目录信息写入到扇区中
    
    WriteHDLinear((info.LBA_START + unuse_sec) * 512,&item,sizeof(struct dir_item));
    // 将这个根目录inode写入扇区中
    root.file_size = sizeof(struct dir_item); // 修改文件大小
    WriteHDLinear(LBA_to_ADDR(sb.inode_array_start),&root,sizeof(struct inode));
}

//得到分区信息
PartInfor* getinfo(char * name){
    // 路径名字分割数组； 最长10个 每个最大为16
    char path_split[10][16]={0};
    // 得到分割数组 以及 路径长度：最小为1
    int path_deep = split(name,'/',path_split,10);

    if(path_deep>=10){
        return NULL;
    }
    path_split[0][1]=0;

    for(int i =0;i<10;i++){
        if(strcmp(parts[i].Name,path_split[0])==0){
            return &parts[i];
        }
    }
}


//得到超级块
struct superblock* Getsuperblock(char *p){
    struct superblock *s = (struct superblock*)malloc(sizeof(struct superblock));
    PartInfor * info = getinfo(p);
    getsuperblock(s,info);

    return s;
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
        strcpy(path_split[path_deep-1],".");
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
    return search_file_inode(&s,part_mes,path_split,1,path_deep,root);
}

// 方法：创建文件夹
void do_mkdir(char * parent,char * name){
    inode *  parent_dir = getinode(parent);
    

    if(parent_dir->attr & ATTR_DIR != ATTR_DIR){
        panic("no dir!");
    }
    // 如果是文件夹,则先创建一个目录文件出来
    inode *dir = do_mkfile(parent,name,ATTR_DIR);
    // 然后写入目录初始数据 . id / .. id
    struct dir_item items[2];
    memset(items,0,sizeof(dir_item)*2);
    strcpy(items[0].name,"..");
    strcpy(items[1].name,".");
    items[0].id=parent_dir->id;
    items[1].id=dir->id;
    struct superblock *s = Getsuperblock(parent);

    do_write(s,dir,items,2 * sizeof(struct dir_item));

}


//方法：创建文件
inode* do_mkfile(char * dir,char * file_name,u16 attr){
    //先得到目标目录的inode
    inode *dir_inode = getinode(dir);
    // 如果给定目录不存在，或则不是目录
    if(dir_inode==NULL||(dir_inode->attr & ATTR_DIR) !=ATTR_DIR){
        return;
    }
    //分配一个inode
    struct superblock *s = Getsuperblock(dir);

    inode * node = allocate_inode(s,file_name,0,attr);

    //初始化一个目录项
    struct dir_item item;
    item.id = node->id;
    strcpy(item.name,file_name);

    //计算目录项的开始扇区
    int sector_index = dir_inode->file_size / 512;
    

    // 如果没有扩展映射
    if(sector_index <7){
        u32 addr_start = dir_inode->data_list[sector_index] * 512 + dir_inode->file_size % 512;
        WriteHDLinear(addr_start,&item,sizeof(struct dir_item));
    }
    dir_inode->file_size+=sizeof(struct dir_item);
    update_inode(s,dir_inode);
    update_inode(s,node);
    return node;
}

void do_write(struct superblock*s,inode * node,u8 *buf,int len){
    // 得到相要写的文件所属磁盘的超级块


    // 得到文件大小
    int fs = node->file_size;
    // 得到字节在扇区中的偏移
    int byte_offset = fs % 512;
    // 得到我们应该写的起始扇区
    int list_offset = fs / 512;
    // 如果字节偏移大于0，那么我们先把当前扇区填充完
    Sector sec_temp;
    u32 *extend_list;
    //更新文件大小
    node->file_size+=len; 
    if(byte_offset>0){
        //填充的数量
        int first_write_c = 512 - byte_offset;
        if(first_write_c>len){
            first_write_c = len;
        }
        //填充的位置
        int start=512;
        int sec = 0;
        if(list_offset>6){
            //如果大于最大长度，我们需要在寻找扩展列表里
            ReadHDLBA(node->data_list[7],&sec_temp,1);
            // 读取扩展列表
            extend_list = (u32*)&sec_temp;
            // 得到还未填充的完的那个扇区
            sec=extend_list[list_offset-7];
        }else{
            sec = node->data_list[list_offset];
        }
        // 开始写数据
        WriteHDLinear(sec*512+byte_offset,buf,first_write_c);
        buf+=first_write_c;
        len-=first_write_c;
    }
    show_str_format(5,5,"addd%x",node->file_size);

    if(len==0){
        return;
    }
    // 我们即将要写的扇区序列数量
    int size = len % 512==0?len/512:(len/512)+1;
    // 然后开始分配扇区

    int * sec_write=allocate_sector(s,node,&size);

    // 然后我们直接遍历给出的扇区序列进行写数据
    for(int i =0;i<size;i++){
        int sec_wrt = sec_write[i];
        if(sec_wrt==node->data_list[7]){
            continue;
        }
        int w_size=len;
        if(w_size>512){
            w_size=512;
            WriteHDLBA(sec_wrt,buf,1);
            len-=w_size;
        }else{
            WriteHDLinear(sec_wrt*512,buf,w_size);
            len-=w_size;
        }
        buf+=w_size;
    }

    update_inode(s,node);
}



//方法：打开文件
int do_open(char * filename,int mode){
    inode *i = getinode(filename);
    struct superblock * s = Getsuperblock(filename);

    if(i!=NULL){
        show_str_format(5,5,"items:%s",i->name);
    }else
    {
        show_str_format(0,6,"no items");
    }
}


