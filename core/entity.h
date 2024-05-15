#pragma once

#include "state.h"

enum
{
    DARKEN_UPDATEPOLICY_STATE,
    DARKEN_UPDATEPOLICY_STATEXTOR,
    DARKEN_UPDATEPOLICY_XTORSTATE,
    DARKEN_UPDATEPOLICY_XTOR,
};

typedef struct de_entity
{
    void (*update)();
    de_state *state;
    de_state *xtor;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_init(de_entity *const, de_state *const);
de_entity *de_entity_set(de_entity *const, de_state *const);
void de_entity_policy(de_entity *const, unsigned);