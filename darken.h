#pragma once

#include "config.h"

typedef void *(*de_state)(void *);

typedef struct
{
    de_state state;
    de_state destructor;
    uint8_t data[];
} de_entity;

typedef struct
{
    de_entity **items;
    uint16_t size;
    uint16_t capacity;
    uint16_t itemSize;
} de_manager;

void darken_init(de_manager *, uint16_t);
de_entity *darken_new(de_manager *, de_state);
void darken_update(de_manager *);
void darken_reset(de_manager *);
void darken_end(de_manager *);
