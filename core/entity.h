#pragma once

#include "../config/darken.h"
#include "state.h"

typedef struct de_manager de_manager;

enum
{
    DARKEN_ENTITY_UPDATEPOLICY_STATE,
    DARKEN_ENTITY_UPDATEPOLICY_STATEXTOR,
    DARKEN_ENTITY_UPDATEPOLICY_XTORSTATE,
    DARKEN_ENTITY_UPDATEPOLICY_XTOR,
};

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

void de_entity_set(de_entity *const, de_state *const);
void de_entity_update(de_entity *const);
unsigned de_entity_delete(de_entity *const);
void de_entity_updatePolicy(de_entity *const, unsigned char);