#include "cutil.h"
#include "type.h"
Cursor GetCursor(){
    Cursor cursor;
    u16 cursors = getcursor();
    cursor.x = cursors % 80;
    cursor.y = cursors / 80;
    return cursor;
}

void print(char *str){
    Cursor cursor=GetCursor();
    display_str(str,cursor.x,cursor.y);
}

u16 u8_and_u8(u8 high,u8 low){
    u16 res = ((u16)high<<8)|((u16)low);
    return res;
}
u32 u16_and_u16(u16 high,u16 low){
    u32 res = ((u32)high<<16)|((u32)low);
    return res;
}

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

void clean_screen(){
    clear();
    SetCursor(0,0);
}
char* itoa(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
	unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
	int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
	//获取要转换的整数的绝对值
	if(radix==10&&num<0)//要转换成十进制数并且是负数
	{
		unum=(unsigned)-num;//将num的绝对值赋给unum
		str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
	}
	else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
	//转换部分，注意转换后是逆序的
	do
	{
		str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
		unum/=radix;//unum去掉最后一位
 
	}while(unum);//直至unum为0退出循环
 
	str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
	//将顺序调整过来
	if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
	else k=0;//不是负数，全部都要调整
 
	char temp;//临时变量，交换两个值时用到
	for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
	{
		temp=str[j];//头部赋值给临时变量
		str[j]=str[i-1+k-j];//尾部赋值给头部
		str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
	}
 
	return str;//返回转换后的字符串
}

void show_str_format(int x,int y,char *str,...){
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