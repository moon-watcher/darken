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

de_entity *de_entity_new(const de_state *const);
void de_entity_delete(de_entity *const);