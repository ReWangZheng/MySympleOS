#ifndef ERROR
#define ERROR
#define assert(exp) if(exp);\
        else assertion_failure(#exp,__FILE__,__BASE_FILE__,__LINE__)

#include "cutil.h"


void panic(char * str);
void assertion_failure(char *exp,char *file,char * base_file,int line);

#endif