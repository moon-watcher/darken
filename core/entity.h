#pragma once

#include "../config/entity.h"
#include "../libs/culist.h"

typedef culist de_manager;

typedef struct de_entity
{
#if DARKEN_ENTITY_INNERDATA > 0
    unsigned char innerdata[DARKEN_ENTITY_INNERDATA];
#endif

    de_state *state;
    de_state *xtor;
    de_state_f update;
    de_manager *manager;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_new(const de_state *const);
void de_entity_delete(de_entity *const);