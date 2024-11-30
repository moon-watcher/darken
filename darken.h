#pragma once

#include "debug.h"
#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken(de_state (*)(), unsigned);

#define DE_CAST(DATA, TYPE) ((TYPE *const)DATA)
#define DE_DATA(DATA, TYPE, VAR) TYPE *const VAR = DE_CAST(DATA, TYPE)
