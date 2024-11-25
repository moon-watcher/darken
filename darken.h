#pragma once

#include "debug.h"
#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken_loop(de_state, unsigned);
void darken_break();

void de_manager_init(de_manager *const, unsigned);

de_entity *de_manager_newEntity(de_manager *const);
int de_manager_deleteEntity(de_manager *const, de_entity *const);

#define DE_CAST(DATA, TYPE) ((TYPE *const)DATA)
#define DE_DATA(DATA, TYPE, VAR) TYPE *const VAR = DE_CAST(DATA, TYPE)

#define DE_LIST_COUNT(THIS) THIS.list.count
#define DE_LIST_CAPACITY(THIS) THIS.list.capacity
