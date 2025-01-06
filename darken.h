#pragma once

#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

#include "uclist/uclist.h"

de_entity *darken_init(void *(*)(unsigned short), unsigned);
void darken_loop(de_entity *const, de_state_f);

#define DE_CAST(ENTITY, TYPE) ((TYPE *const)ENTITY->data)
#define DE_DATA(ENTITY, TYPE, VAR) TYPE *const VAR = DE_CAST(ENTITY, TYPE)
