#pragma once

#include "../config/darken.h"
#include "state.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
#if DARKEN_ENTITY_FIXEDDATA > 0
    unsigned char fixeddata[DARKEN_ENTITY_FIXEDDATA];
#endif

#if DARKEN_ENTITY_TEMPDATA
    void *tempdata;
#endif

    de_state *state;
    de_state *xtor;
    de_state_f update;
    de_manager *manager;

#if DARKEN_ENTITY_DATA
    unsigned char data[]; // Bytes for casting data & components
#endif
} de_entity;

de_entity *de_entity_set(de_entity *const, const de_state *const);
de_entity *de_entity_update(de_entity *const);
int de_entity_delete(de_entity *const);
void de_entity_updateType(de_entity *const, unsigned char);