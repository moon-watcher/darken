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
    de_state_f update;
    de_state *state;
    de_state *xtor;
    void *tempdata;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_init(de_entity *const, de_state *);
de_entity *de_entity_set(de_entity *const, de_state *);
void de_entity_updatePolicy(de_entity *const, unsigned);
void de_entity_update(de_entity *const);
void de_entity_destroy(de_entity *const);

#define DE_ENTITY_SIZE sizeof(de_entity)