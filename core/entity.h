#pragma once

#include "state.h"
#include "manager.h"

typedef struct de_entity
{
    void (*update)();
    void (*leave)();
    void (*destructor)();
    de_state *state;
    de_manager *manager;
    int ctrl;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

de_entity *de_entity_set(de_entity *const, de_state *const);
de_entity *de_entity_delay(de_entity *const);
de_entity *de_entity_delete(de_entity *const);