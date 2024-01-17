#pragma once

#include "entity.h"

typedef struct darken
{
    de_entity *scene;
    char loop;
} darken;

void darken_init(darken *const, unsigned);
void darken_looper(darken *const, const de_state *const);
void darken_state(darken *const, const de_state *const);
void darken_break(darken *const);
void darken_loop(darken *const);
void darken_end(darken *const);