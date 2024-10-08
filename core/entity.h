#pragma once

#include "state.h"
#include "manager.h"

typedef struct de_entity
{
    void (*update)();
    void (*destructor)();
    de_state *state;
    de_manager *manager;
    int status;
    unsigned char data[]; // Bytes for casting data & components
} de_entity;

void de_entity_set(de_entity *const, de_state *);
void de_entity_delete(de_entity *const);
void de_entity_skipUpdate(de_entity *const);