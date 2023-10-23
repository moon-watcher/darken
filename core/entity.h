#pragma once

#include "../config/darken.h"
#include "state.h"

typedef struct de_manager de_manager;

typedef struct de_entity
{
#if DARKEN_ENTITY_FIXEDDATA > 0
    unsigned char fixeddata[DARKEN_ENTITY_FIXEDDATA];
#endif

#if DARKEN_ENTITY_PTRDATA
    void *ptrdata;
#endif

    de_state *state;
    de_state *xtor;
    de_state_f update;
    de_manager *manager;

#if DARKEN_ENTITY_DATA
    unsigned char data[]; // Bytes for casting data & components
#endif
} de_entity;

unsigned int de_entity_set(de_entity *const, const de_state *const);
void de_entity_enter(de_entity *const);
void de_entity_update(de_entity *const);
void de_entity_leave(de_entity *const);
unsigned int de_entity_delete(de_entity *const);
void de_entity_destruct(de_entity *const);

