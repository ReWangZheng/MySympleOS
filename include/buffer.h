#ifndef BUFFER
#define BUFFER
#include "type.h"

typedef struct Buffer
{
    int read_cursor;
    int append_cursor;
    int size;
    int max_size;
    u8 *buf;
}Buffer;
Buffer* MakeBuffer(int bufsize);
u8 get(Buffer * buffer);
void append(Buffer* buffer,u8 code);
#endif