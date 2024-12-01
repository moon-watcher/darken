#pragma once

#include "debug.h"
#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken(de_state (*)(), unsigned);

#define DE_CAST(ENTITY, TYPE) ((TYPE *const)ENTITY->data)
#define DE_DATA(ENTITY, TYPE, VAR) TYPE *const VAR = DE_CAST(ENTITY, TYPE)
