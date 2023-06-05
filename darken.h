#pragma once

#include <genesis.h>

#include "libs/UFL.h"

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
#include "helpers/scene.h"
#include "helpers/state.h"
#include "helpers/entity.h"
#include "helpers/system.h"

typedef struct deEntityState_t
{
    deEntity_t *entity;
    deState_t *state;
} deEntityState_t;

int darken(const deState_t *const);
void darken_end(int);