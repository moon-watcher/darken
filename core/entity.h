#pragma once

#include "state.h"

typedef struct de_entity
{
    de_state *state;
    de_state *newState;
    de_state_f destructor;
    int delay;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_set(de_entity *const, de_state *const);
void de_entity_delay(de_entity *const);
void de_entity_update(de_entity *const);
void de_entity_destroy(de_entity *const);