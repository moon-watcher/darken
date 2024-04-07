#pragma once

#include "entity.h"

typedef struct darken
{
    de_entity *scene;
    char loop;
} darken;

void de_main_init(darken *, unsigned);
void de_main_looper(darken *, de_state *);
void de_main_state(darken *, de_state *);
void de_main_break(darken *);
void de_main_loop(darken *);
void de_main_end(darken *);