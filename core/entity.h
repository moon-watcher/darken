#pragma once

#include "../config/darken.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
    de_state *state;
    de_manager *manager;

#if DARKEN_ENTITY_STATEDATA
    void *state_data;
#endif

#if DARKEN_ENTITY_FIXEDDATA > 0
    unsigned char fixed_data[DARKEN_ENTITY_FIXEDDATA];
#endif

#if DARKEN_ENTITY_DATA
    unsigned char data[]; // Bytes for casting data & components
#endif
} de_entity;

#if DARKEN_ENTITY_DATA
#define deh_entity_data(ENTITY, TYPE, DATA) \
    TYPE *const DATA = (TYPE *const)&ENTITY->data

#define deh_entity_var(DATA, TYPE, VAR1) \
    TYPE *const VAR1 = (TYPE *const)&DATA->VAR1

#define deh_entity_varas(DATA, TYPE, VAR1, VAR2) \
    TYPE *const VAR2 = (TYPE *const)&DATA->VAR1
#endif

de_entity *de_entity_new(const de_state *const);
void de_entity_delete(de_entity *const);