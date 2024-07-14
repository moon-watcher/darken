#pragma once

#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken_loop(unsigned, de_state *);
void darken_break();

#define DE_DATA(THIS, TYPE) ((TYPE *const)THIS.data)
