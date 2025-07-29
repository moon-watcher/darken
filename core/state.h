#pragma once

#include "entity.h"

typedef void *(*de_state)();

void *de_state_empty();
void *de_state_exec(de_state, de_entity *const);
void *de_state_update(de_state, de_entity *const);
