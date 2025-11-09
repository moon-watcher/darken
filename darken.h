#pragma once

#include "config.h"

typedef void *(*de_state)(void *const);

typedef struct
{
    de_state state;
    de_state destructor;
    unsigned char data[];
} de_entity;

typedef struct
{
    de_entity **items;
    unsigned short size;
    unsigned short capacity;
    unsigned short itemSize;
} de_manager;

void darken_init(de_manager *const, unsigned short);
de_entity *darken_new(de_manager *const, de_state);
void darken_update(de_manager *const);
void darken_reset(de_manager *const);
void darken_end(de_manager *const);
