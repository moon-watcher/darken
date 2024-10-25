#pragma once

#include "debug.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

// Delete
typedef void (*de_state)();

void darken_loop(void (*[static 1])(), unsigned);
void darken_break();

#define DE_CAST(DATA, TYPE) ((TYPE *const)DATA)
#define DE_DATA(DATA, TYPE, VAR) TYPE *const VAR = DE_CAST(DATA, TYPE)

#define DE_LIST_COUNT(THIS) THIS.list.count
#define DE_LIST_CAPACITY(THIS) THIS.list.capacity
