#pragma once

#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

de_entity *darken_init(void *(*)(unsigned short), unsigned);
void darken_loop(de_entity *const, de_state);

#define darken_cast(ENTITY, TYPE) ((TYPE *const)ENTITY->data)
#define darken_data(ENTITY, TYPE, VAR) TYPE *const VAR = darken_cast(ENTITY, TYPE)
