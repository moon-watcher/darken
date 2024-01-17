#pragma once

#include "../config/darken.h"
#include "state.h"

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

#if DARKEN_ENTITY_DATA
    unsigned char data[]; // Bytes for casting data & components
#endif
} de_entity;

de_entity *de_entity_set(de_entity *const, de_state *const);
void de_entity_updatePolicy(de_entity *const, unsigned);