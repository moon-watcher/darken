#pragma once

#include <genesis.h>

//#define LOG(MESSAGE)
#define LOG(MESSAGE) kprintf("%s:%d %s", __FILE__, __LINE__, MESSAGE);
