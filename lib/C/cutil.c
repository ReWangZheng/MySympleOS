#include "cutil.h"
#include "type.h"
//得到光标
Cursor GetCursor(){
    Cursor cursor;
    u16 cursors = getcursor();
    cursor.x = cursors % 80;
    cursor.y = cursors / 80;
    return cursor;
};
//连接两个字
u16 u8_and_u8(u8 high,u8 low){
    u16 res = ((u16)high<<8)|((u16)low);
    return res;
}
//连接两个16位字
u32 u16_and_u16(u16 high,u16 low){
    u32 res = ((u32)high<<16)|((u32)low);
    return res;
}

//数字转字符 10进制
char * itoc(char *str,int number){
    char numberchar[]={'0','1','2','3','4','5','6','7','8','9'};
    int i=0;
    int yushu =0;
    int len=0;
    int temp = number;
    do
    {
        temp /= 10; //10 1 0
        len++;
    } while (temp > 0);
    str[len]=0;
    do
    {
        yushu =number % 10;
        number /= 10;
        str[--len]=numberchar[yushu];
    } while (number > 0);
    return str;
}
//数字转字符 16进制
char * itoh(char *str,int number){
    char numberchar[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    int i=0;
    int yushu =0;
    int len=2;
    int temp = number;
    do
    {
        temp /= 16; //10 1 0
        len++;
    } while (temp > 0);
    str[len]=0;
    do
    {
        yushu =number % 16;
        number /= 16;
        str[--len]=numberchar[yushu];
    } while (number > 0);
    str[0] = '0';
    str[1] = 'x';
    return str;
}
//清屏，并且设置光标在0,0
void clean_screen(){
    u16 *addr = 0xb8000;
    for(int i =0;i<25*80;i++){
        *(addr+i)=0;
    }
    SetCursor(0,0);
}
// 打印字符串
void show_str_format(int x,int y,const char *str,...){
    u32 * paragram = &str;
    u16 *display = 0xb8000;
    int i = 0;
    int cursor = 0;
    int state = 0;
    display = display + y*80 + x;
    char temp[30];
    while(str[i]!=0){
        if(str[i]=='%' && str[i+1] == 'd'){
            //itoa(*(++paragram),ttt,10);
            itoc(temp,*(++paragram));
            for(int j =0;j<30;j++){
                if(temp[j]==0){
                    break;
                }else
                {
                    *(display + cursor) = u8_and_u8(0x07,temp[j]); 
                    cursor++;        
                }
            }
            i+=2;
        }else if(str[i]=='%' && str[i+1] == 'x'){
            itoh(temp,*(++paragram));
            for(int j =0;j<30;j++){
                if(temp[j]==0){
                    break;
                }else
                {
                    *(display + cursor) = u8_and_u8(0x07,temp[j]); 
                    cursor++;        
                }
            }
            i+=2;
        }else if(str[i]=='%' && str[i+1] == 's'){
            paragram++;
            char * s = (*paragram);
            for(int j =0;j<30;j++){
                if(s[j]==0){
                    break;
                }else
                {
                    *(display + cursor) = u8_and_u8(0x07,s[j]); 
                    cursor++;        
                }
            }
            i+=2;

        }
        else
        {
            *(display + cursor) = u8_and_u8(0x07,str[i]); 
            i++;
            cursor++;
        }
    }     
}

//测试比特位
int bit_test(u8 input,int flag){
    if(input==0xff){
        return -1;
    }
    for(int i =0;i<8;i++){
        int test = ((input >> i) & 1);
        if(flag==0 && flag == test){
            return i;
        }else if(flag==1 && flag==test){
            return i;
        }
    }
    return -1;
}

int bit(u8 *buf,int x){
    int byte_index = x/8;
    int bit_offset = x %8;
    u8 byte = *(buf+byte_index);
    return (byte>>bit_offset) & 1;
}


//字符串方法
void strcp(char * dst,char * src){
    while (1)
    {
        *dst = *src;
        if((*src)==0){
            break;
        }
        src++;
        dst++;
    }
}
int strcmp(char * dst,char * src){
    while (1)
    {
        char c_dst = (*dst);
        char c_src = (*src);
        if(c_dst==0 && c_src==0){
            return 0;
        }
        if(c_dst-c_src!=0){
            return c_dst-c_src;
        }
        dst++;
        src++;
    }
}

int split(char * src,char splitch,char buf[][16],int max){
    int buf_cursor =0;
    int buf_offset =0;
    int index = 0;
    int res=1;
    while (1)
    {
        char ch = src[index++];
        if(ch==0){
            break;
        }
        if(ch == splitch){
            buf_cursor++;
            buf_offset=0;
            res++;
        }else{
            if(buf_cursor>=max){
                break;
            }
            buf[buf_cursor][buf_offset++]=ch;
        }
    }
    return res;
}
