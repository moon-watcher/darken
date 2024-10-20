#pragma once

#include "core/debug.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken_loop(de_state *const, unsigned);
void darken_break();

#define DE_CAST(DATA, TYPE) ((TYPE *const)DATA)
#define DE_DATA(DATA, TYPE, VAR) TYPE *const VAR = DE_CAST(DATA, TYPE)

#define DE_LIST_COUNT(THIS) THIS.list.count
#define DE_LIST_CAPACITY(THIS) THIS.list.capacity