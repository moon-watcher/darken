#pragma once

#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

de_entity *darken_init(void *(*)(unsigned short), unsigned);
void darken_loop(de_entity *const, de_state);
