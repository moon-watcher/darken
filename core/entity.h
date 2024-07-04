#pragma once

#include "../libs/uplist.h"

typedef uplist de_manager;

#include "state.h"

typedef struct de_entity
{
    de_state *state;
    de_state *newState;
    de_state_f destructor;
    de_manager *manager;
    int delay;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_set(de_entity *const, de_state *const);
void de_entity_delay(de_entity *const);
int de_entity_delete(de_entity *const);