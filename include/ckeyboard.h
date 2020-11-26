#pragma once
#include "type.h"
#include "process.h"

void append_code(KeyCode code);
KeyCode fetch_code();
void InitKeyBuf();
int keybuffersize();
void KeyProcess_enter();
