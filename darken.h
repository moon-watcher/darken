#pragma once

#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

#define DARKEN_SIZE(SIZE) sizeof(de_entity) + SIZE

void darken(de_entity *const, de_state);
