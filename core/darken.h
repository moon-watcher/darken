#pragma once

#include "state.h"
#include "entity.h"

typedef struct
{
    de_entity *looper;
    de_entity *entity;
    unsigned char loop;
} darken;

void darken_init(darken *const);
void darken_loop(darken *const);
void darken_break(darken *const);
void darken_end(darken *const);
void darken_set_looper(darken *const, de_state *const);
void darken_set_state(darken *const, de_state *const);