#pragma once

#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

void darken(const de_state *const);
de_entity *darken_init(const de_state *const);
void darken_end(de_entity *const);
void darken_state(de_entity *const, const de_state *const);
void darken_update(de_entity *const);
void darken_loop(de_entity *const);
void darken_break(de_entity *const);