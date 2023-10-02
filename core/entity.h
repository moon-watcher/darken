#pragma once

#include "../config/darken.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
#if DARKEN_ENTITY_INNERDATA > 0
    unsigned char innerdata[DARKEN_ENTITY_INNERDATA];
#endif

#if DARKEN_STATE_STATEDATA
    void *statedata;
#endif

    de_state *state;
    de_state_f update;
    de_manager *manager;

#if DARKEN_ENTITY_DATA
    unsigned char data[]; // Bytes for casting data & components
#endif
} de_entity;

de_entity *de_entity_new(const de_state *const);
void de_entity_delete(de_entity *const);