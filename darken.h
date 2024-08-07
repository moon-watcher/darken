#pragma once

#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken_loop(de_state *const, unsigned);
void darken_break();

#define DE_DATA(THIS, TYPE) ((TYPE *const)THIS.data)
