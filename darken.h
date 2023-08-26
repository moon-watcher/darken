#pragma once

#include "core/state.h"
#include "core/entity.h"
#include "core/manager.h"
#include "core/system.h"

typedef struct
{
    int exitCode;
    de_entity *entity;
    de_state *state;
} darken;

void darken_init(darken *const, const de_state *const);
void darken_update(darken *const);
void darken_state(darken *const, const de_state *const);
void darken_loop(darken *const);
void darken_end(darken *const);
