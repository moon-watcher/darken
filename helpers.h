#pragma once

#include "config/darken.h"

// #if DARKEN_ENTITY_DATA
#define deh_entity_data(ENTITY, TYPE, DATA) \
    TYPE *const DATA = (TYPE *const)&ENTITY->data

#define deh_entity_var(DATA, TYPE, VAR) \
    TYPE *const VAR = (TYPE *const)&DATA->VAR

#define deh_entity_varas(DATA, TYPE, VAR, AS) \
    TYPE *const AS = (TYPE *const)&DATA->VAR
// #else
// #define deh_entity_data
// #define deh_entity_var
// #define deh_entity_varas
// #endif