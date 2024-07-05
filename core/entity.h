#pragma once

#include "state.h"
#include "../libs/uplist.h"

typedef uplist de_manager;

typedef struct de_entity
{
    de_state *state;
    de_state *newState;
    void (*destructor)();
    de_manager *manager;
    int ctrl;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_set(de_entity *const, de_state *const);
void de_entity_delay(de_entity *const);
void de_entity_delete(de_entity *const);