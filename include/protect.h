#ifndef PROTECT
#define PROTEXT
#include "type.h"
#include "cprotect.h"
void LoadGDT(GDT * addr);
void LoadIDT(IDT * addr);
void GetGDT(GDT * gdt);
#endif