#pragma once

#include "../config/entity.h"
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
    de_state_f update;
    de_state *state;
    de_state *xtor;

#if DARKEN_ENTITY_TEMPDATA
    void *tempdata;
#endif

#if DARKEN_ENTITY_DATA
    unsigned char data[]; // Bytes for casting data & components
#endif
} de_entity;

de_entity *de_entity_init(de_entity *const, de_state *);
de_entity *de_entity_set(de_entity *const, de_state *);
void de_entity_updatePolicy(de_entity *const, unsigned);