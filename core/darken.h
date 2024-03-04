#pragma once

#include "entity.h"

typedef struct darken
{
    de_entity *scene;
    char loop;
} darken;

void darken_init(darken *, unsigned);
void darken_looper(darken *, de_state *);
void darken_state(darken *, de_state *);
void darken_break(darken *);
void darken_loop(darken *);
void darken_end(darken *);