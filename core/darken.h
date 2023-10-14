#pragma once

#include "state.h"
#include "entity.h"

typedef struct
{
    // de_entity *looper;
    de_entity *scene;
    unsigned char loop;
} darken;

void darken_init(darken *const);
// void darken_looper(darken *const, de_looper *const, unsigned int);
void darken_scene(darken *const, de_scene *const, unsigned int);
void darken_loop(darken *const);
void darken_break(darken *const);
void darken_end(darken *const);