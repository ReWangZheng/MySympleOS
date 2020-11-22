#ifndef PROTECT
#define PROTEXT
#include "type.h"
#include "cprotect.h"
void LoadGDT(GDT * addr);
void LoadIDT(IDT * addr);
void GetGDT(GDT * gdt);
void LoadLDT(u32 ldt_sel);
u32 getEAX();
u32 getEBX();
u32 getECX();
u32 getEDX();
u32 getESP();
u32 getSS();
u32 getDS();
u32 getCS();
u32 getEFLAGE();
#endif