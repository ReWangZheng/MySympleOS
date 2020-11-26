#include "buffer.h"
Buffer* MakeBuffer(int bufsize){
    Buffer * res = (Buffer*)malloc(sizeof(Buffer));
    res->buf =(u8*)malloc(bufsize);
    res->size = 0;
    res->max_size = bufsize;
    res->read_cursor = -1;
    res->append_cursor = 0;
    return res;
}
#include "util.h"
void append(Buffer* buffer,u8 code){
    //判断缓冲区满了没有
    show_str_format(0,3,"max_size:%d r:%d  a:%d  ",buffer->max_size,buffer->read_cursor+1,buffer->append_cursor);
    if(buffer->size>=buffer->max_size){
        return;
    }
    buffer->buf[buffer->append_cursor] = code;
    buffer->append_cursor = (buffer->append_cursor + 1) % buffer->max_size;
    buffer->size++;
}

u8 get(Buffer * buffer){
    if(buffer->read_cursor ==-1||buffer->read_cursor == buffer->append_cursor){
        return 0;
    }
    buffer->read_cursor = (buffer->read_cursor+1) % buffer->max_size;
    u8 code = buffer->buf[buffer->read_cursor];
    return code;
}


