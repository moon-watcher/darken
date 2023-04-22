#pragma once

#include <genesis.h>

#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

#include "api/darkEn.h"
#include "api/system.h"
#include "api/state.h"
#include "api/entity.h"
#include "api/manager.h"

#include "helpers/helpers.h"

int  darken(const deState_t *const);
void darken_end(int);